#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "printf-stdarg.h"
#include "sam3x8e.h"
#include "can_controller.h"
#include "can_interrupt.h"

int can_send_flag = 0;
void SysTick_Handler()
{
	static uint32_t can_rx_timer = 0;
	if (can_rx_timer >= 1000) 
	{
		can_send_flag = 1;
		can_rx_timer = 0;
	}
	can_rx_timer++;
}
void Systick_Init(void)
{
	SysTick->LOAD = 84000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

int main()
{
    PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
    SystemInit();
	Systick_Init();
    PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
	
	uint32_t can_br = CAN_BR_PHASE2(4) | CAN_BR_PHASE1(3) | CAN_BR_PROPAG(3) | CAN_BR_SJW(1) | CAN_BR_BRP(125); //125
	
	CAN_MESSAGE message_rx = {0};
	CAN_MESSAGE message_tx = {0};
	message_tx.id = 0;
	message_tx.data[0] = 'B';
	message_tx.data_length = 1;
	
    configure_uart();
	
	can_init_def_tx_rx_mb(can_br);
	
	if (can_br == 1){
		printf("Failed to init CAN\n");
	} else{
		printf("CAN successfully initialized");
	}
	
    while (1)
    {
		if (can_send_flag) {
			can_send_flag = 0;
			can_send(&message_tx, 0);
		}
		
		//int a = can_receive(&message_rx, 0);
		//for (int i=0; i < 8; i++) {
			//printf(" %d |", message_rx.data[i]);
		//}
		//printf("id: %d \n\r", message_rx.id);
    }
}


void delay_ms(uint32_t milliseconds) {
	// Calculate the number of clock cycles needed for the delay
	#define CLOCK_FREQ 8400000 // 8.4 MHz
	uint32_t cycles = (CLOCK_FREQ / 1000) * milliseconds;

	// Perform the delay using a loop (marked as volatile to prevent optimization)
	volatile uint32_t i;
	for (i = 0; i < cycles; i++) {
		// Do nothing (just wait)
	}
}
