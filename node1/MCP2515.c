#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "MCP2515.h"
#include "spi.h"

uint8_t mcp2515_read(uint8_t address)
{
    uint8_t result;
    CS_ENABLE; // Select CAN - controller
    SPI_Transmit_Recieve(MCP_READ); // Send read instruction
    SPI_Transmit_Recieve(address); // Send address

    result = SPI_Transmit_Recieve(0) ; // Read result

    CS_DISABLE;
    return result;
}

void mcp2515_write(uint8_t address, uint8_t data)
{
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_WRITE);
    SPI_Transmit_Recieve(address);
    SPI_Transmit_Recieve(data);
    CS_DISABLE;
}


void mcp2515_req_to_send(uint8_t buf)
{
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_REQUEST_TO_SEND | (1 << buf));
    CS_DISABLE;
}

uint8_t mcp2515_read_status()
{
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_READ_STATUS);
    uint8_t res = SPI_Transmit_Recieve(0);
    CS_DISABLE;

    return res;
}

void mcp2515_modify_bit(uint8_t address, uint8_t mask, uint8_t data)
{
    CS_ENABLE;
    SPI_Transmit_Recieve(MCP_BIT_MODIFY);
    SPI_Transmit_Recieve(address);
    SPI_Transmit_Recieve(mask);
    SPI_Transmit_Recieve(data);
    CS_DISABLE;
}

void mcp2515_reset()
{
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
    // More initialization
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
}
