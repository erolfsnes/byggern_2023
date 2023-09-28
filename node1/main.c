/* -------- BYGGERN -------- */

#include "OLED_menu.h"
#define F_CPU 4915200 // Clock speed
#define BAUD 9600	// Baud rate
#define MYUBRR F_CPU/16/BAUD-1

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "UART_driver.h"
#include "SRAM_driver.h"
#include "ADC_header.h"
#include "OLED_driver.h"
#include "OLED_menu.h"

int main(void)
{
	// Driver initializations
	USART_Init(MYUBRR);
	fdevopen(USART_Transmit, USART_Receive);
	SRAM_Init();
	ADC_Init();
	OLED_init();
	
	// Declarations
	volatile adc_data data = {0};
	pos_t pos_data;
	
	pos_calibrate(&data.x_offs, &data.y_offs);
    OLED_reset();

    menu_init();
    menu_print(1);

	
	while(1)
	{
		_delay_ms(10); // Making the while-loop a bit slower for reading purposes - runs 100 times a second
		
		ADC_Read(&data);
		pos_data = pos_read(&data);
		// joystick_dir direction = dir_read(&pos_data);

		//printf("%d\t %d\t %d\t %d\t %d\t\r\n", data.ch1, data.ch2, data.ch3, data.ch4, direction);
// 		for (int i = 0; i < 8; i++)
// 		{
// 			for (int j = 0; j < 128; j++)
// 			{
// 				OLED_pos(i, j);
// 				if (j % 2) {
// 					OLED_write_data(0xFF);
// 				} else {
// 					OLED_write_data(0x00);
// 				}
// 			}
// 		}
//
// 	
		
	
		/* 
		EXERCISE 1.6
	
		DDRA = (1 << DDA0);	// Defining pin A0 as output pin

		If PORTA is written logic one when the pin is configured as an output pin, the port pin is driven
		high (one). If PORTA is written logic zero when the pin is configured as an output pin, the port
		pin is driven low (zero).
		
		_delay_ms(100);
		PORTA &= ~(1 << PA0); // Setting pin low
		_delay_ms(100);
		PORTA = (1 << PA0);	// Setting pin high
		*/
		
		
		/* 
		EXERCISE 1.10-11
		
		USART_Transmit(USART_Receive());
		printf("Hello");
		_delay_ms(500);
		*/
		
		/* 
		EXERCISE 2.1
		DDRA = (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3) | (1 << DDA4) | (1 << DDA5) | (1 << DDA6) | (1 << DDA7);
		DDRE = (1 << DDE1);
		
		PORTE = (1 << PE1);
		PORTA = (1 << PA7);	// Setting pin high
		PORTE = 0;
		PORTA = 0;
		USART_Receive();
		PORTE = (1 << PE1);
		PORTE = 0;
		USART_Receive();
		*/
		
		/*EXERCISE 2.3
		// 2Y NAND CS ADC
		// PC3 -> SRAM, PC2 -> ADC
	
		DDRC = (1 << DDC3) | (1 << DDC2);
		PORTC = (1 << PC2);
		*/
	}
}

