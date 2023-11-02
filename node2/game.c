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

#define K_P 0.5
#define K_I 1.0
#define K_D 1.0
#define T 0.05

uint32_t uw_tick = 0;
extern uint8_t score;

void game_init(void)
{
	PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
	SystemInit();
	Systick_Init();
	PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;

	WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
	
	configure_uart();
	
	PWM_init();
	PWM_set_duty_cycle(1.5);
	ADC_init();
	DAC_init();
	motor_init();
	PID_init(K_P, K_I, K_D, T);
}

void get_score(uint32_t adc_value)
{
	static uint32_t last_time = 0;
	static uint32_t last_time_score = 0;
	static uint32_t last_value = 2500;
	static uint32_t can_score = 0;
	
	if(adc_value < 500){
		printf("ADC val: %d\r\n", adc_value);
	}
	
	if ((abs(last_value - adc_value) < 100) && (adc_value > 300)) {
		if ((uw_tick - last_time_score) > 500) {
			can_score = 1;
			last_time_score = uw_tick;
		}
	} 

	if(adc_value<25){
		if((uw_tick - last_time > 3000) && can_score){
			score--;
			last_time = uw_tick;
			can_score = 0;
			//printf("Score: %d, Time: %d\n\r", score, SysTick->VAL);
		}
		last_time_score = uw_tick;
	}
	
	last_value = adc_value;
}