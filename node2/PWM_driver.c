#include "PWM_driver.h"
#include "sam.h"
#include "printf-stdarg.h"
#include <stdint.h>

// Flatkabel connection til ping-pong board:
// Svart = servosig, grå = 5V, blå = ground, hvit = solenoid, lilla = solenoid, grønn = NC, gul = photo+, oransje = photo-, rød = IR+, brun = IR-

void PWM_init(void)
{
	// Disabling PIO for pins 44 and 45
	PIOC->PIO_PDR |= PIO_PDR_P19 | PIO_PDR_P18;
	// Selecting A and B peripherals for pins 44 and 45
	PIOC->PIO_ABSR |= PIO_ABSR_P19 | PIO_ABSR_P18;
	
	// Enable clock
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;
	// Configure clock
	PWM->PWM_CLK = PWM_CLK_PREA(0x7) | PWM_CLK_PREB(0x7) | PWM_CLK_DIVA(1) | PWM_CLK_DIVB(1);
	//PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_PREB(0) | PWM_CLK_DIVA(84) | PWM_CLK_DIVB(84);
	
	// Enable channel 5
	PWM->PWM_ENA = PWM_ENA_CHID5;
	// Set channel mode
	PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
	// Set channel period
	PWM->PWM_CH_NUM[5].PWM_CPRD = 13125;

}

void PWM_set_duty_cycle(float duty_cycle)
{
		
	if (duty_cycle <= 0.91) {
		duty_cycle = 0.91;
	}	
	else if (duty_cycle >= 2.05) {
		duty_cycle = 2.05;
	}

	PWM->PWM_CH_NUM[5].PWM_CDTY = (duty_cycle/20) * 13125;

}

float mapValue(int8_t input) {
	static int8_t last_value = 0;
	
	// Check if out of bounds
	if (input > 100) input = 100;
	else if (input < -100) input = -100;
	
	if(input < last_value+5 && input > last_value-5){
		input = last_value;
	}
	last_value = input;
	
	// Define the input and output ranges
	float in_min = -100.0, in_max = 100.0;
	float out_min = 0.9, out_max = 2.1;

	// Map the input value to the output range
	float output = out_min + (input - in_min) * (out_max - out_min) / (in_max - in_min);
	return output;
}