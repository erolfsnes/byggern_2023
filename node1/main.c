/* ---- BYGGERN ------- */

#define F_CPU 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "UART_driver.h"
#include "SRAM_header.h"

//ttyS4

int main(void)
{
	
	USART_Init(MYUBRR);
	fdevopen(USART_Transmit, USART_Receive);
	
	//SRAM_init();
	//SRAM_test();
	
	
	
	while(1)
	{
		
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
		
		/* EXERCISE 2.3 */
		// 2Y NAND CS ADC
		
	
		DDRC = (1 << DDC3) | (1 << DDC2);
		PORTC = (1 << PC2);
	}
	
}

