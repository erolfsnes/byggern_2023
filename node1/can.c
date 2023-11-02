#include "can.h"
#include "ADC_header.h"
#include "MCP2515.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

extern can_msg_t can_rx_msg;
extern int can_it_flag;
extern int can_score_recieved;


void can_init() {
    mcp2515_write(CAN_RXB0CTRL, 3 << 5); // Turn filter off
    mcp2515_write(CAN_INTE, 0x01);
    
    cli();
    DDRE &= ~(1 << PE0);
    EMCUCR &= ~(1 << ISC2);
    GICR |= (1 << INT2);
    sei();

    can_it_flag = 0;
}

uint8_t can_transmit(can_msg_t msg)
{
    mcp2515_write(CAN_ID_L, (msg.id & 0x7) << 5); // Set can ID
    mcp2515_write(CAN_ID_H, (msg.id & ~0x7) >> 3); // Set can ID

    if (msg.len > 8) {
        return 1;
    }
    mcp2515_write(CAN_DATA_LEN, msg.len); //Set msg length
     

    uint8_t address = CAN_LOAD_DATA;
    for (int i = 0; i < msg.len; i++) {
        mcp2515_write(address, msg.data[i]);
        address++;
    }
    
    // Request to send buffer 0
    mcp2515_req_to_send(0);

    return 0;
}

ISR(INT2_vect) {
    cli();
    can_rx_msg = can_recieve_it();
    printf("ID: %d\r\n", can_rx_msg.id);
    switch (can_rx_msg.id) {
        case CAN_ID_LIVES:
            can_score_recieved = 1;
            break;
    }
    GIFR |= (1 << INTF2);
    sei();
}

can_msg_t can_recieve_it(void) {
    
    uint8_t int_status;

    uint8_t data_len = mcp2515_read(RX_DATA_LEN_BUF0);
    data_len &= 0x3;

    can_msg_t msg = {0};
    uint8_t addr = CAN_RXBUF0_LL;

    for (int i=0; i < data_len; i++) {
        msg.data[i] = mcp2515_read(addr);
        addr++;
    }

    uint8_t id_l, id_h;

    id_l = mcp2515_read(CAN_RX_ID_L) >> 5;
    id_h = mcp2515_read(CAN_RX_ID_H);

    msg.id = (id_h << 3) | (id_l);

    int_status = mcp2515_read(CAN_INTF);
    // clear interupt 
    mcp2515_write(CAN_INTF, int_status & ~1);

//    for (int i=0; i < 8; i++) {
//        printf(" %d |", msg.data[i]);
//    }
//    printf("id: %d \n\r", msg.id);
    can_it_flag = 1;

   return msg;
}

can_msg_t can_recieve(void) {
    mcp2515_write(CAN_RXB0CTRL, 3 << 5); // Turn filter off
    
    uint8_t int_status;

    for (int i=0; i < 10; i++) {
        int_status = mcp2515_read(CAN_INTF);
        printf("%d", int_status & 0x01);
        if (int_status & 0x01) {
            mcp2515_write(CAN_INTF, int_status & ~1);
            break;
        }
        else {
            _delay_ms(10);
        }
    }
    uint8_t data_len = mcp2515_read(RX_DATA_LEN_BUF0);
    data_len &= 0x3;
    printf("rec len: %d\n\r", data_len);

    can_msg_t msg = {0};
    uint8_t addr = CAN_RXBUF0_LL;
    for (int i=0; i < data_len; i++) {
        msg.data[i] = mcp2515_read(addr);
        addr++;
    }

    uint8_t id_l, id_h;

    id_l = mcp2515_read(CAN_RX_ID_L);
    id_h = mcp2515_read(CAN_RX_ID_H);

    msg.id = id_h | (id_l >> 5);

    for (int i=0; i < 8; i++) {
        printf(" %d |", msg.data[i]);
    }
    printf("id: %d \n\r", msg.id);

    return msg;
}


void can_send_joystick_data(pos_t *data, uint8_t button_status) {
    can_msg_t msg = {0};
    msg.data[0] = (uint8_t)data->x;
    msg.data[1] = (uint8_t)data->y;
    msg.data[2] = button_status;
    msg.id = CAN_TX_JOYSTICK;
    msg.len = 3;

    can_transmit(msg);
}
