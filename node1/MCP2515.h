#ifndef MCP2515_H_
#define MCP2515_H_

#include <stdint.h>

/* MCP registers */
#define MCP_READ 0x03
#define MCP_WRITE 0x02
#define MCP_REQUEST_TO_SEND 0x80
#define MCP_READ_STATUS 0xA0
#define MCP_BIT_MODIFY 0x05
#define MCP_RESET 0xC0
#define MCP_CANSTAT 0x0E
#define MCP_CANCTRL 0x0F


#define MODE_MASK 0xE0
#define MODE_CONFIG (1 << 7)
#define MODE_LOOPBACK (1 << 6)


uint8_t mcp2515_read(uint8_t address);
void mcp2515_write(uint8_t address, uint8_t data);
void mcp2515_req_to_send(uint8_t buf);
uint8_t mcp2515_read_status();
void mcp2515_modify_bit(uint8_t address, uint8_t mask, uint8_t data);
void mcp2515_reset();
uint8_t mcp2515_init();

#endif /* ifndef MCP2515_H_ */
