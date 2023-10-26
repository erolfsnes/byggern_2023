#include "PWM_driver.h"
#include "sam.h"
#include "printf-stdarg.h"
#include <stdint.h>

void ADC_init(void)
{
	// FLippa bitså
	ADC->ADC_WPMR &= (0xFFFFFFFF << ADC_WPMR_WPEN);
	
	// Enable clock
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;
	
	// Freerunning mode
	ADC->ADC_MR = (ADC_MR_FREERUN);
	
	// Enable chanel 0
	ADC->ADC_CHER = ADC_CHER_CH0;
	
	ADC->ADC_CR = ADC_CR_START;
	
	//ADC->ADC_WPMR |= 1;
	
}

uint32_t ADC_read() 
{
	uint32_t adcval = ADC->ADC_CDR[0];
	
	return adcval;
}