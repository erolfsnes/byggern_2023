#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "printf-stdarg.h"
#include "sam3x8e.h"
#include "can_controller.h"
#include "can_interrupt.h"

int main()
{
	
    PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
    SystemInit();
    PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
	
	uint32_t can_br = CAN_BR_PHASE2(5) | CAN_BR_PHASE1(4) | CAN_BR_PROPAG(3) | CAN_BR_SJW(3) | CAN_BR_BRP(102); //125
	
	CAN_MESSAGE message = {0};
	message.id = 0;
	message.data[0] = 'A';
	message.data_length = 1;
	
    configure_uart();

    PMC->PMC_PCER0 |= (1 << ID_PIOA);
    PIOA->PIO_PER |= (PIO_PA19 | PIO_PER_P20);

    PIOA->PIO_OER |= (PIO_PA19 | PIO_PA20);

    PIOA->PIO_SODR = (PIO_PA19 | PIO_PA20);
	
	printf("Hello World1\n\r");
	
	can_init_def_tx_rx_mb(can_br);
	
	printf("Hello World2\n\r");
	
	if (can_br == 1){
		printf("Failed to init CAN\n");
	} else{
		printf("CAN successfully initialized");
	}
	
	//for (int i=0; i < 8; i++) {
		//printf(" %d |", message.data[i]);
	//}
	//printf("id: %d \n\r", message.id);
	
    while (1)
    {
		can_send(&message, message.id);
    }
}
