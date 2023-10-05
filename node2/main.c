#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"

#include "sam.h"


int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    configure_uart();

    while (1)
    {
    
        printf("Hello World\n\r");
         
        /* code */
    }
    
}
