#include "PWM_driver.h"
#include "sam.h"
#include "printf-stdarg.h"
#include <stdint.h>
#include <stdlib.h>

uint32_t uw_tick = 0;

uint8_t score(uint32_t adc_value)
{
	static uint8_t counter = 0;
	static uint32_t last_time = 0;
	static uint32_t last_time_score = 0;
	static uint32_t last_value = 2500;
	static uint32_t can_score = 0;
	
	if ((abs(last_value - adc_value) < 100) && (adc_value > 1500)) {
		if ((uw_tick - last_time_score) > 500) {
			can_score = 1;
			last_time_score = uw_tick;
		}
	} 

	
	if(adc_value<1000){
		if((uw_tick - last_time > 3000) && can_score){
			counter++;
			last_time = uw_tick;
			can_score = 0;
			printf("Score: %d, Time: %d\n\r", counter, SysTick->VAL);
		}
		last_time_score = uw_tick;
	}
	
	last_value = adc_value;

}