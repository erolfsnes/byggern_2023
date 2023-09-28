#include <avr/io.h>
#include <stdint.h>
#include "spi.h"


void SPI_Init(void)
{
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCLK);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR); // | (1 << SPR0);
}

uint8_t SPI_Transmit_Recieve(uint8_t cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)))
        ;

    return SPDR;
}


