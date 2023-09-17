/* ---- BYGGERN ------- */

#define F_CPU 4915200 // Clock speed
#define BAUD 9600	// Baud rate
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "UART_driver.h"
#include "SRAM_header.h"
#include "ADC_header.h"

//ttyS4

int main(void)
{
	adc_data data = {0};
	pos_t pos_data;
	USART_Init(MYUBRR);
	fdevopen(USART_Transmit, USART_Receive);
	SRAM_Init();
	ADC_Init();
	pos_calibrate(&data.x_offs, &data.y_offs);
	
	while(1)
	{
		ADC_Read(&data);
		pos_data = pos_read(&data);
		joystick_dir direction = dir_read(&pos_data);
		printf("%d\t %d\t %d\t %d\t %d\t\r\n", data.ch1, data.ch2, data.ch3, data.ch4, direction);
		
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

