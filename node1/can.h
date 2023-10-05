#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include "MCP2515.h"

#define CAN_ID_H 0x31
#define CAN_ID_L 0x32
#define CAN_RX_ID_H 0x61
#define CAN_RX_ID_L 0x62
#define CAN_DATA_LEN 0x35
#define CAN_LOAD_DATA 0x36
#define CAN_RXB0CTRL 0x60
#define CAN_INTE 0x2B
#define CAN_INTF 0x2C
#define RX_DATA_LEN_BUF0 0x65
#define CAN_RXBUF0_LL 0x66

#define CAN_RXBUF0_INTE 0x01


typedef struct {
    uint8_t id;
    uint8_t len;
    uint8_t data[8];

} can_msg_t;

extern can_msg_t can_rx_msg;
extern int can_it_flag;

void can_init();
uint8_t can_transmit(can_msg_t msg);
can_msg_t can_recieve(void);
can_msg_t can_recieve_it(void);

#endif /* ifndef CAN_H_ */
