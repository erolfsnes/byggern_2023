#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "ADC_header.h"
#define BASE_ADDRESS_ADC 0x1400;

void ADC_Init(void)
{
	// Define PD4 as output pin
	DDRD = (1 << DDD4);
	
	// Configure Time/Counter Control Register to PWM mode
	TCCR3A = (1 << WGM30) | (1 << COM3A1) | (1 << COM3A0); 
	TCCR3B = (1 << CS10);
	OCR3A = 127;
	
	// Enable interrupt
	sei();
}

void ADC_Read(adc_data *data)
{
	volatile char *ext_mem = (char*) BASE_ADDRESS_ADC;
	
	// First write a value to the ADC to set the write signal low
	ext_mem[0] = 0;
	
	// Wait for conversion to finish (conversion time = 3.8 us)
	_delay_ms(10);
	
	// Obtain the converted value for each channel and store it in the struct
	data->ch1 = ext_mem[0]; // Joystick X-axis
	data->ch2 = ext_mem[0]; // Joystick Y-axis
	data->ch3 = ext_mem[0]; // Left slider
	data->ch4 = ext_mem[0]; // Right slider
	
	printf("%d\t %d\t %d\t %d\t\r\n", data->ch1, data->ch2, data->ch3, data->ch4);
}

// rel. between voltage and joystick angle: f = 40x - 100 (x = voltage)

void pos_calibrate(uint8_t *x_offs, uint8_t *y_offs)
{
	double x_sum = 0;
	double y_sum = 0;
	adc_data data = {0};
	
	for (int i = 0; i<= 10; i++) {
		ADC_Read(&data);
		x_sum += data.ch1;
		y_sum += data.ch2;
	}
	
	*x_offs = x_sum / 10;
	*y_offs = x_sum / 10;
}

pos_t pos_read(uint8_t x_val, uint8_t y_val)
{
	
}