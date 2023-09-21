#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "OLED_header.h"
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