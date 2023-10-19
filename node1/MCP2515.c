#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "MCP2515.h"
#include "spi.h"

uint8_t mcp2515_read(uint8_t address)
{
    uint8_t result;
    CS_DISABLE;
    CS_ENABLE; // Select CAN - controller
    SPI_Transmit_Recieve(MCP_READ); // Send read instruction
    SPI_Transmit_Recieve(address); // Send address

    result = SPI_Transmit_Recieve(0) ; // Read result

    CS_DISABLE;
    return result;
}

void mcp2515_write(uint8_t address, uint8_t data)
{
    CS_DISABLE;
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_WRITE);
    SPI_Transmit_Recieve(address);
    SPI_Transmit_Recieve(data);
    CS_DISABLE;
}


void mcp2515_req_to_send(uint8_t buf)
{
    CS_DISABLE;
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_REQUEST_TO_SEND | (1 << buf));
    CS_DISABLE;
}

uint8_t mcp2515_read_status()
{
    CS_DISABLE;
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_READ_STATUS);
    uint8_t res = SPI_Transmit_Recieve(0);
    CS_DISABLE;

    return res;
}

void mcp2515_modify_bit(uint8_t address, uint8_t mask, uint8_t data)
{
    CS_DISABLE;
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_BIT_MODIFY);
    SPI_Transmit_Recieve(address);
    SPI_Transmit_Recieve(mask);
    SPI_Transmit_Recieve(data);
    CS_DISABLE;
}

void mcp2515_reset()
{
    CS_DISABLE;
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_RESET);
    CS_DISABLE;
}

void mcp2515_set_mode(uint8_t mode)
{
    mcp2515_modify_bit(MCP_CANCTRL, MODE_MASK, mode);
}

uint8_t mcp2515_init()
{
    uint8_t value ;
    SPI_Init() ; // Initialize SPI
    mcp2515_reset() ; // Send reset - command

    value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK ) != MODE_CONFIG) {
        printf("MCP2515 is NOT in configuration mode after reset!\n\r");
        return 1;
    }
    else {
        printf("MCP is in MODE_CONFIG, %d\n\r", value);
    }

    mcp2515_write(MCP_CNF1, (1 << 7) | (11 << 0));  // BRP = 3 // Syncronization jump = 3 Tq

    mcp2515_write(MCP_CNF2, (1 << 7) | (1 << 6) | (3 << 3) | (3 << 0)); // Length of PS2 is determined by the PHSEG2[2:0] bits of CNF3 // PS1 = 4 // Propagation Segment Length bits
					
    mcp2515_write(MCP_CNF3, 4 <<0 ); // PS2 Length bits

    // More initialization
#ifndef MCP_LOOPBACK

    mcp2515_set_mode(MODE_NORMAL);

    value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK ) != MODE_NORMAL) {
        printf("MCP2515 is NOT in MODE_NORMAL mode after reset!\n\r");
        return 1;
    }
    else {
        printf("MCP is in MODE_NORMAL %d\n\r", value);
    }
    return 0;
 
#endif /* ifndef MCP_LOOPBACK */
#ifdef MCP_LOOPBACK

    mcp2515_set_mode(MODE_LOOPBACK);

    value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK ) != MODE_LOOPBACK) {
        printf("MCP2515 is NOT in LOOPBACK_MODE mode after reset!\n\r");
        return 1;
    }
    else {
        printf("MCP is in MODE_LOOPBACK %d\n\r", value);
    }
    return 0;

#endif /* ifdef MCP_LOOPBACK */
}
