#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "printf-stdarg.h"
#include "sam3x8e.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "PWM_driver.h"
#include "ADC_driver.h"
#include "game.h"
#include "motor_driver.h"
#include "DAC_driver.h"
#include "PID_driver.h"

void SysTick_Handler();
void Systick_Init(void);

int adc_read_flag = 0;
int can_send_flag = 0;
int can_receive_flag = 0;
int score_flag = 0;

extern uint32_t uw_tick;
uint8_t score = 0;

CAN_MESSAGE message_rx = {0};

int main()
{
	uint32_t can_br = CAN_BR_PHASE2(4) | CAN_BR_PHASE1(3) | CAN_BR_PROPAG(3) | CAN_BR_SJW(1) | CAN_BR_BRP(125);
	
	CAN_MESSAGE message_tx = {0};
	volatile joystick_data data = {0};
	PID_parameters parameters = {0};
	
	uint8_t can_error = can_init_def_tx_rx_mb(can_br);
	
	if (can_error){
		printf("Failed to init CAN: %d\n", can_error);
	} else{
		printf("CAN successfully initialized");
	}
	
	game_init();
	
    while (1)
    {	
		if(adc_read_flag){
			uint32_t adc_value = ADC_read();
			if (!data.button) get_score(adc_value);

			if (score_flag && can_send_flag){
				can_send_flag = 0;
				CAN_MESSAGE message_score = {0};
				message_score.id = 11;
				message_score.data[0] = score;
				message_score.data_length = 1;
				can_send(&message_score, 0);
				if (score <= 0) {
					score_flag = 0;
				}
			}
		}
		
		if (can_receive_flag){
			can_receive_flag = 0;
			switch (message_rx.id){
				case CAN_JOYSTICK_ID:
					data.x = (int8_t)message_rx.data[0];
					data.y = (int8_t)message_rx.data[1];
					data.button = message_rx.data[2];
					//printf("%d\t %d\t %d\t \r\n", data.x, data.y, data.button);
					PWM_set_duty_cycle(mapValue(-data.x));
					motor_write(data.y, data.button);
					//printf("%d\r\n", motor_read());
					//printf("mapValue: %f\n\r", mapValue(data.x));
					break;
				case 11:
					printf("ABCDEFG");
					score_flag = 1;	
					score = message_rx.data[0];
					printf("SCORE: %d\n\r", score);
			}
		}
    }
}

void SysTick_Handler()
{
	uw_tick++;
	
	static int adc_read_counter = 0;
	if (adc_read_counter > 50){
		adc_read_flag = 1;
		adc_read_counter = 0;
	}

	adc_read_counter++;
	
	static uint32_t can_rx_timer = 0;
	if (can_rx_timer >= 250)
	{
		can_send_flag = 1;
		can_rx_timer = 0;
	}
	can_rx_timer++;
}

void Systick_Init(void)
{
	SysTick->LOAD = 84000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(uint32_t milliseconds) 
{
	// Calculate the number of clock cycles needed for the delay
	#define CLOCK_FREQ 8400000 // 8.4 MHz
	uint32_t cycles = (CLOCK_FREQ / 1000) * milliseconds;

	// Perform the delay using a loop (marked as volatile to prevent optimization)
	volatile uint32_t i;
	for (i = 0; i < cycles; i++) {
		// Do nothing (just wait)
	}
}
