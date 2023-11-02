/* -------- BYGGERN -------- */

#include "menu.h"
#include "tim.h"
#define F_CPU 4915200 // Clock speed
#define BAUD 9600	// Baud rate
#define MYUBRR F_CPU/16/BAUD-1

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "UART_driver.h"
#include "SRAM_driver.h"
#include "ADC_header.h"
#include "OLED_driver.h"
#include "menu.h"
#include "spi.h"
#include "MCP2515.h"
#include "can.h"

can_msg_t can_rx_msg;
int can_it_flag = 0;
volatile int can_tx_flag = 0;
int can_score_recieved = 0;

int main(void)
{
	// Driver initializations
	USART_Init(MYUBRR);
	fdevopen(USART_Transmit, USART_Receive);
    printf("Start...");
	SRAM_Init();
	ADC_Init();
	OLED_init();
    Timer1_Init();

	// Declarations
	adc_data data = {0};
	pos_t pos_data;
	
	pos_calibrate(&data.x_offs, &data.y_offs);
    
    OLED_reset();
    mcp2515_init();
    can_init();

    can_msg_t msg = {0};
    msg.len = 2;
    msg.id = 1;
    msg.data[0] = 255;
    msg.data[1] = 0x0F;
    can_msg_t r_msg = {0};

    menu_init();
    main_menu();
    _delay_ms(10);
    uint16_t last_score = 0;
	while(1)
	{

        if (can_tx_flag) {
            ADC_Read(&data);
            uint8_t button_status = joy_button_read();
		    pos_data = pos_read(&data);

            can_send_joystick_data(&pos_data, button_status);
           can_tx_flag = 0;
        }
		
        if (can_score_recieved) {
            uint8_t lives = can_rx_msg.data[0];
            printf("lives: %d \n\r", lives - 1);
            if (lives != 1) {
                if (last_score != score) {
                    print_score(score, lives-1);
                    last_score = score;
                }
            }
            else {
                print_game_over();
                while (!joy_button_read());
                main_menu();
            }
        }

	}
}

