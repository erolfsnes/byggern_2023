#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "printf-stdarg.h"
#include "sam3x8e.h"

int main()
{
	
	CAN0->CAN_BR = CAN_BR_PHASE2(5) | CAN_BR_PHASE1(5) | CAN_BR_PROPAG(2) | CAN_BR_SJW(1) | CAN_BR_BRP(167);
	
	
    PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
    SystemInit();
    PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    configure_uart();
	
    printf("Hello World\n\r");
    PMC->PMC_PCER0 |= (1 << ID_PIOA);
    PIOA->PIO_PER |= (PIO_PA19 | PIO_PER_P20);

    PIOA->PIO_OER |= (PIO_PA19 | PIO_PA20);

    PIOA->PIO_SODR = (PIO_PA19 | PIO_PA20);
    
    while (1)
    {
		
    }
}
