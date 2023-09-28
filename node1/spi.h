#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#define DDR_SPI DDRB
#define DD_CS DDB4
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCLK DDB7
#define CS_ENABLE PORTB &= ~(1 << DD_CS)
#define CS_DISABLE PORTB |= (1 << DD_CS)

void SPI_Init(void);
uint8_t SPI_Transmit_Recieve(uint8_t cData);

#endif /* ifndef SPI_H_ */
