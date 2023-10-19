
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tim.h"

#define F_CPU 4915200 
extern volatile int can_tx_flag;

void Tim1_IRQ_Handler();

void Timer1_Init() {
    // Set CTC mode with OCR1A as the top value
    TCCR1B = (1 << WGM12);
    
    OCR1A =  4915;//4915 - 1;
    
    // Enable the Timer 1 Output Compare A interrupt
    TIMSK = (1 << OCIE1A);
    
    // Start the timer with a prescaler of 64
     TCCR1B |= ((1 << 0));//  | (1 << CS11));
}


void Tim1_IRQ_Handler()
{
    static uint32_t i = 0;
    if (i >= 50)
    {
        i = 0;
        can_tx_flag = 1;
    }
    i++;
}

ISR(TIMER1_COMPA_vect) {
    cli();
    Tim1_IRQ_Handler();
    TIFR |= (1 << OCF1A);
    sei();
}
