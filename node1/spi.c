#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "spi.h"


void SPI_Init(void)
{
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCLK) | (1 << PB4);// | (0 << DD_MISO);
    // DDR_SPI &= ~((1 << PB3) | (1 << PB6) | (1 << PB1) | (1 << PB2));
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); //| (1 << SPR1);// | (1 << SPR0);
}

uint8_t SPI_Transmit_Recieve(uint8_t cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    uint16_t timeout = 0;
    static uint16_t timeout_counter = 0;
    while(!(SPSR & (1 << SPIF))) {
        timeout++;
        if (timeout == 65535) {
            printf("\n\rSPI timout reached\n\r");
            timeout_counter++;
            printf("Timeout counter: %d\r\n", timeout_counter);
            return 0;
        }
    }
    return SPDR;
}


