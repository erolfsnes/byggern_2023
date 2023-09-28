#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

typedef struct{
	volatile uint8_t ch1;
	volatile uint8_t ch2;
	volatile uint8_t ch3;
	volatile uint8_t ch4;
	volatile int16_t x_offs;
	volatile int16_t y_offs;
} adc_data;

typedef enum{
	NEUTRAL,
	LEFT,
	UP,
	RIGHT,
	DOWN,
} joystick_dir;

typedef struct{
	int8_t x;
	int8_t y;
} pos_t;

void ADC_Init(void);
void ADC_Read(volatile adc_data *data);
void pos_calibrate(int16_t *x_offs, int16_t *y_offs);
pos_t pos_read(adc_data *data);
joystick_dir dir_read(pos_t *pos_data);
uint8_t joy_button_read(void);

#endif
