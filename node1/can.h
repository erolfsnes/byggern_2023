#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include "MCP2515.h"

#define CAN_ID_H 0x31
#define CAN_ID_L 0x32
#define CAN_DATA_LEN 0x35
#define CAN_LOAD_DATA 0x36


typedef struct {
    uint8_t id;
    uint8_t len;
    uint8_t data[8];

} can_msg_t;

void can_init();
uint8_t can_transmit(can_msg_t msg);

#endif /* ifndef CAN_H_ */
