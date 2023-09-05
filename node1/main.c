/*
 * Exercise1.c
 *
 * Created: 31/08/2023 10:57:15
 * Author : Erlend
 */ 

#define F_CPU 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "UART_driver.h"

//ttyS4

int main(void)
{
	
	USART_Init(MYUBRR);
	fdevopen(USART_Transmit, USART_Receive);
	unsigned char character = 'g';
	
	while(1)
	{
		
		/* 
		EXERCISE 3.6
	
		DDRA = (1 << DDA0);	// Defining pin A0 as output pin

		If PORTA is written logic one when the pin is configured as an output pin, the port pin is driven
		high (one). If PORTA is written logic zero when the pin is configured as an output pin, the port
		pin is driven low (zero).
		
		_delay_ms(100);
		PORTA &= ~(1 << PA0); // Setting pin low
		_delay_ms(100);
		PORTA = (1 << PA0);	// Setting pin high
		*/
		
		
		/* EXERCISE 3.10-11 */
		
		//USART_Transmit(USART_Receive());
		printf("Hello");
		//_delay_ms(500);
	}
	
}

void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)))
		;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)))
		;
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Flush(void)
{
	unsigned char dummy;
	while (UCSR0A & (1<<RXC0) ) dummy = UDR0;
}