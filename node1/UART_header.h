#ifndef UART_H_
#define UART_H_

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);
void USART_Flush(void);

#endif