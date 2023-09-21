#ifndef OLED_H_
#define OLED_H_

typedef struct{
	uint8_t row;
	uint8_t column;
} OLED_pos_t;

void OLED_init(void);
void OLED_pos(uint8_t row, uint8_t column);
#endif