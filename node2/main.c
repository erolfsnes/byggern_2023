#include <stdio.h>
#include <stdarg.h>
#include "component/component_wdt.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"

#include "sam.h"


int main()
{
    PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
    SystemInit();
    PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;

    // WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    configure_uart();

    printf("Hello World\n\r");
    PMC->PMC_PCER0 |= (1 << ID_PIOA);
    PIOA->PIO_PER |= (PIO_PA19 | PIO_PER_P20);

    PIOA->PIO_OER |= (PIO_PA19 | PIO_PA20);

    PIOA->PIO_SODR = (PIO_PA19 | PIO_PA20);
    
    while (1)
    {
        WDT->WDT_CR = WDT_CR_KEY_Pos | WDT_CR_WDRSTT;
    
   //     printf("Hello World\n\r");
         
        /* code */
    }
}
