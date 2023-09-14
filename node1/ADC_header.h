#ifndef ADC_H_
#define ADC_H_

typedef struct{
	uint8_t ch1;
	uint8_t ch2;
	uint8_t ch3;
	uint8_t ch4;
	uint8_t x_offs;
	uint8_t y_offs;
} adc_data;

typedef struct{
	int8_t x;
	int8_t y;
} pos_t;

void ADC_Init(void);
void ADC_Read(adc_data *data);
void pos_calibrate(uint8_t *x_offs, uint8_t *y_offs);
pos_t pos_read(uint8_t x_val, uint8_t y_val);

#endif