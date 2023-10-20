#include "PWM_driver.h"
#include "sam.h"
#include "printf-stdarg.h"
#include <stdint.h>


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
	
	// Enable channel 5
	PWM->PWM_ENA = PWM_ENA_CHID5;
	// Set channel period
	PWM->PWM_CH_NUM[5].PWM_CPRD = 13125;
	//REG_PWM_CPRD5 = 13125;
	// Set channel mode
	PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;

}

void PWM_set_duty_cycle(float duty_cycle)
{
	if (duty_cycle <= 0.9) {
		duty_cycle = 0.9;
	}	
	else if (duty_cycle >= 2.1) {
		duty_cycle = 2.1;
	}
	float dc = duty_cycle/1000;
	duty_cycle = duty_cycle/1000;
	PWM->PWM_CH_NUM[5].PWM_CDTY = -1/(PWM_FREQ*(dc*PWM_PERIOD-PWM_PERIOD));
	printf("ttttttttttttttttttttttt %d", -1/(PWM_FREQ*(dc*PWM_PERIOD-PWM_PERIOD)));
}