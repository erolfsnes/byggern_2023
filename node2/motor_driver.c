#include "PWM_driver.h"
#include "sam.h"
#include "printf-stdarg.h"
#include <stdint.h>
#include <stdlib.h>
#include "DAC_driver.h"
#include "motor_driver.h"

#define DIR PIO_PD10
#define EN PIO_PD9
#define SEL PIO_PD2
#define NOT_RST PIO_PD1
#define NOT_OE PIO_PD0
#define BUTTON PIO_PD6
#define DO_MASK (0xFF << 1)

extern uint32_t uw_tick;

void motor_init(void)
{
	PIOD->PIO_PER |= DIR | EN | SEL | NOT_RST | NOT_OE | BUTTON;
	PIOD->PIO_OER |= DIR | EN | SEL | NOT_RST | NOT_OE | BUTTON;
	
	PIOD->PIO_SODR = BUTTON;
	
	PIOC->PIO_PER |= DO_MASK;
	PIOC->PIO_ODR |= DO_MASK;
	
	PMC->PMC_PCER0 |= PMC_PCER0_PID14;
	PMC->PMC_PCER0 |= PMC_PCER0_PID13;
	
}

void motor_write(int8_t input, uint8_t button)
{
	//if(button){
		//PIOD->PIO_CODR = BUTTON;
		//uint32_t t = uw_tick;
		//delay_us(150000);
		//PIOD->PIO_SODR = BUTTON;
	//}
	
	static uint32_t last_time = 0;
	static uint32_t can_shoot = 1;
	
	if (button && can_shoot)
	{
		PIOD->PIO_CODR = BUTTON;
		can_shoot = 0;
		last_time = uw_tick;
	}
	if ((uw_tick - last_time) > 500) {
		can_shoot = 1;
	}
	
	else if ((uw_tick - last_time) > 50) {
		PIOD->PIO_SODR = BUTTON;
	}

	if (input >= 10) {
		DAC_write(scale_to_motor(input));
		PIOD->PIO_SODR = DIR | EN;
	} else if(input <= 10) {
		DAC_write(scale_to_motor(-input));
		PIOD->PIO_CODR = DIR;
		PIOD->PIO_SODR = EN;
	} else {
		PIOD->PIO_CODR = EN;
	}
}

int16_t motor_read(void)
{
	PIOD->PIO_CODR = NOT_OE | SEL;
	delay_us(20);
	PIOD->PIO_CODR = NOT_RST;
	PIOD->PIO_SODR = NOT_RST;
	uint8_t message_hb = (PIOC->PIO_PDSR & DO_MASK) >> 1;
	PIOD->PIO_SODR = SEL;
	delay_us(20);
	uint8_t message_lb = (PIOC->PIO_PDSR & DO_MASK) >> 1;	
	PIOD->PIO_SODR = NOT_OE;
	
	uint16_t message = (message_hb << 8) | message_lb;
	
	return (int16_t)message;
}

void delay_us(uint32_t microseconds)
{
	// Calculate the number of clock cycles needed for the delay
	#define CLOCK_FREQ 8400000 // 8.4 MHz
	uint32_t cycles = (CLOCK_FREQ / 1000000) * microseconds;

	// Perform the delay using a loop (marked as volatile to prevent optimization)
	volatile uint32_t i;
	for (i = 0; i < cycles; i++) {
		// Do nothing (just wait)
	}
}

uint16_t scale_to_motor(int8_t input) 
{
	// Define the min and max values for uint16_t
	const uint16_t min_val = 0;
	const uint16_t max_val = 65535;

	// Ensure input is within the expected range
	if (input < 0) input = 0;
	if (input > 100) input = 100;

	// Scale the input
	return (uint16_t)(min_val + (max_val - min_val) * (input / 100.0));
}