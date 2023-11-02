#include "PWM_driver.h"
#include "sam.h"
#include "printf-stdarg.h"
#include <stdint.h>
#include <stdlib.h>

void DAC_init(void)
{
	// Enable peripheral clock
	PMC->PMC_PCER1 |= PMC_PCER1_PID38;
	
	// Disable write protection
	DACC->DACC_WPMR = (8 << 0x444143);
	DACC->DACC_WPMR &= ~(1 << DACC_WPMR_WPEN);
	
	// Channel and startup selection
	DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_STARTUP_0;
	
	// Channel enable
	DACC->DACC_CHER = DACC_CHER_CH1;
}

void DAC_write(uint16_t data)
{
	DACC->DACC_CDR = data;
}
