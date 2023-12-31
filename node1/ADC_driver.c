#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ADC_header.h"
#include <stdlib.h>
#define BASE_ADDRESS_ADC 0x1400

volatile char *ext_mem_adc = (char*) BASE_ADDRESS_ADC;

void ADC_Init(void)
{
	// Define PD4 as output pin
	DDRD = (1 << DDD4);
	
	// Configure Time/Counter Control Register to PWM mode
	TCCR3A = (1 << WGM30) | (1 << COM3A1) | (1 << COM3A0); 
	TCCR3B = (1 << CS10);
	OCR3A = 127; // When timer's count value reaches the OCR-value, the PWM-output is changed
	
	// Enable interrupt
	sei();
    
    // Define PB0 as input
    DDRB &= ~(1 << DDB0);

}

uint8_t joy_button_read(void)
{
    static uint8_t button_status;
    button_status = (PINB & 1 << PINB0) ? 0: 1;
    
    return button_status;
}

void ADC_Read(volatile adc_data *data)
{
	// First write a value to the ADC to set the write signal low
	ext_mem_adc[0] = 0;
	
	// Wait for conversion to finish (conversion time = 3.8 us)
	_delay_ms(10);
	
	// Obtain the converted value for each channel and store it in the struct
	data->ch1 = ext_mem_adc[0]; // Joystick X-axis
	data->ch2 = ext_mem_adc[0]; // Joystick Y-axis
	data->ch3 = ext_mem_adc[0]; // Left slider
	data->ch4 = ext_mem_adc[0]; // Right slider
}

// rel. between voltage and joystick angle: f = 40x - 100 (x = voltage)

void pos_calibrate(int16_t *x_offs, int16_t *y_offs)
{
	int16_t x_sum = 0;
	int16_t y_sum = 0;
	adc_data data = {0};
	
	for (int i = 0; i < 10; i++) {
		ADC_Read(&data);
		x_sum += data.ch1;
		y_sum += data.ch2;
        printf("ch1: %d\t ch2: %d\n\r", data.ch1, data.ch2);
	}

	
	*x_offs = (x_sum / 10);// - 127;
	*y_offs = (y_sum / 10);// - 127;
}


pos_t pos_read(adc_data *data) {
    int16_t x_pos;
    int16_t y_pos;
    if (data->ch1 > data->x_offs)
    {
         x_pos = (((100 * data->ch1) / (255 - data->x_offs)) - ((100 * data->x_offs) / (255 - data->x_offs)));
    }
    else {
        x_pos = (((100 * data->ch1) / data->x_offs) - 100);
    }

    if (data->ch2 > data->y_offs)
    {
         y_pos = (((100 * data->ch2) / (255 - data->y_offs)) - ((100 * data->y_offs) / (255 - data->y_offs)));
    }
    else {
        y_pos = (((100 * data->ch2) / data->y_offs) - 100);
    }
    pos_t pos;
    pos.x = x_pos;
    pos.y = y_pos;
    return pos;
}

/* pos_t pos_read(adc_data *data)
{	
	// Calculate the scale factors for both sides of the center position
	int32_t scale_x_left = 100 / data->x_offs; // Scale factor for the left side of x
	int32_t scale_x_right = 100 / (255 - data->x_offs); // Scale factor for the right side of x
	int32_t scale_y_up = 100 / data->y_offs; // Scale factor for the upper side of y
	int32_t scale_y_down = 100 / (255 - data->y_offs); // Scale factor for the lower side of y

	// Read the joystick positions
	int32_t x_pos = (int16_t) data->ch1;// - data->x_offs;
	int32_t y_pos = (int16_t) data->ch2;// - data->y_offs;

	// Normalize the positions based on the center position
	int16_t norm_pos_x = x_pos > 127 ? (x_pos * scale_x_right) : (x_pos * scale_x_left);
	int16_t norm_pos_y = y_pos > 127 ? (y_pos * scale_y_down) : (y_pos * scale_y_up);

	// Store transformed x and y data in a struct and return it
	pos_t pos_data;
	pos_data.x = norm_pos_x;
	pos_data.y = norm_pos_y;

	return pos_data;
} */


/* pos_t pos_read(adc_data *data)
{	
	// Transform joystick position data to be in the interval (-100, 100) instead of (0, 255)
	int32_t x_pos = (int16_t) data->ch1 - data->x_offs;
	int32_t y_pos = (int16_t) data->ch2 - data->y_offs;
	int16_t norm_pos_x = (x_pos * 200) / 255 - 100;
	int16_t norm_pos_y = (y_pos * 200) / 255 - 100;
	//int32_t norm_pos_x = ((int32_t)(data->ch1) * (127 - (-127)) / 255-27) + (-127);
	
	// Store transformed x and y data in a struct and return it
	pos_t pos_data;
	pos_data.x = norm_pos_x;
	pos_data.y = norm_pos_y;

	return pos_data;
} */

joystick_dir dir_read(pos_t *pos_data)
{
	// Return joystick direction based on a certain threshold in x and y position
	uint8_t thold = 20;
	
	if (abs(pos_data->x) > abs(pos_data->y)) {
		if (pos_data->x < -thold) return LEFT;
		else if (pos_data->x > thold) return RIGHT;
		else return NEUTRAL;
	}
	else{
		if (pos_data->y < -thold) return DOWN;
		else if (pos_data->y > thold) return UP;
		else return NEUTRAL;
	}
}
