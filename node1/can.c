#include "can.h"
#include "MCP2515.h"
#include <stdint.h>

uint8_t can_transmit(can_msg_t msg)
{
    mcp2515_write(CAN_ID_L, (msg.id<< 5)); // Set can ID
    if (msg.len > 8) {
        return 1;
    }
    mcp2515_write(CAN_DATA_LEN, msg.len); //Set msg length
     

    uint8_t address = CAN_LOAD_DATA;
    for (int i = 0; i < msg.len; i++) {
        mcp2515_write(address, msg.data[i]);
        address++;
    }
    
    // Request to send buffer 1
    mcp2515_req_to_send(1);

    return 0;
}
