#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/pgmspace.h>
#include "OLED_header.h"
#include "fonts.h"
#define BASE_ADDRESS_OLED_COMMAND 0x1000;
#define BASE_ADDRESS_OLED_DATA 0x1200;

OLED_pos_t position = {0};
volatile char *ext_mem_command = (char*) BASE_ADDRESS_OLED_COMMAND;
volatile char *ext_mem_data = (char*) BASE_ADDRESS_OLED_DATA;

void OLED_write_command(char command)
{
	ext_mem_command[0] = command;
}

volatile void OLED_write_data(char data)
{
	ext_mem_data[0] = data;
}

void OLED_init(void)
{
	 OLED_write_command(0xae); // Display off
	 OLED_write_command(0xa1); // Segment remap
	 OLED_write_command(0xda); // Common pads hardware: alternative
	 OLED_write_command(0x12);
	 OLED_write_command(0xc8); // Common output scan direction:com63~com0
	 OLED_write_command(0xa8); // Multiplex ration mode:63
	 OLED_write_command(0x3f);
	 OLED_write_command(0xd5); // Display divide ratio/osc. freq. mode
	 OLED_write_command(0x80);
	 OLED_write_command(0x81); // Contrast control
	 OLED_write_command(0x50);
	 OLED_write_command(0xd9); // Set pre-charge period
	 OLED_write_command(0x21);
	 OLED_write_command(0x20); // Set Memory Addressing Mode
	 OLED_write_command(0x02);
	 OLED_write_command(0xdb); // VCOM deselect level mode
	 OLED_write_command(0x30);
	 OLED_write_command(0xad); // Master configuration
	 OLED_write_command(0x00);
	 OLED_write_command(0xa4); // Out follows RAM content
	 OLED_write_command(0xa6); // Set normal display
	 OLED_write_command(0xaf); // Display on
}

void OLED_pos(uint8_t row, uint8_t column)
{
	OLED_goto_column(column);
	OLED_goto_row(row);
}

void OLED_goto_row(uint8_t row)
{
	if (0 <= row && row < 8)
	{
		position.row = row;
		OLED_write_command(0xB0 | row);
	}
}

void OLED_goto_column(uint8_t column)
{
	if (0 <= column && column < 128)
	{
		position.column = column;
		OLED_write_command(column % 16);
		OLED_write_command(0x10 | (column / 16));
	}
}

void OLED_reset(void)
{
	for (int i = 0; i < 8; i++)
	{
		OLED_clear_line(i);
	}
	//OLED_home();
}

void OLED_home(void)
{
	
}

void OLED_clear_line(uint8_t line)
{
	for (int i = 0; i < 128; i++)
	{
    OLED_pos(line, i);
		OLED_write_data(0x00);
	}
}

void OLED_set_brightness(uint8_t lvl)
{
	if (0 <= lvl && lvl < 256) {
		uint8_t brightness = 255-lvl;
		OLED_write_command(0x81 | brightness);
	}
}

void OLED_set_addressing_mode(OLED_addressing_mode_t mode)
{
	switch (mode)
	{
		case PAGE_ADDRESSING_MODE:
			OLED_write_command(0x20);
			OLED_write_command(0x02);
			break;
		case HORIZONTAL_ADDRESSING_MODE:
			OLED_write_command(0x20);
			OLED_write_command(0x00);
			break;
		case VERTICAL_ADDRESSING_MODE:
			OLED_write_command(0x20);
			OLED_write_command(0x01);
			break;
	}
}

void OLED_putc(char c)
{
	for (int i = 0; i < 8; i++)
	{
		OLED_write_data(pgm_read_byte(&font8[c - 32][i]));
	}
}

void OLED_print(char* data)
{
	while (*data != '\0')
	{
		if ((position.column + 8) > 127)
		{
			if ((position.row + 1) > 7)
			{
				OLED_pos(0, 0);
			}
			else
			{
				OLED_pos(position.row + 1, 0);
			}
		}
		OLED_putc(*data);
		data++;
	}
}