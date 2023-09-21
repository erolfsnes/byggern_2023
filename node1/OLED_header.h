#ifndef OLED_H_
#define OLED_H_

typedef struct{
	uint8_t row;
	uint8_t column;
} OLED_pos_t;

void OLED_init(void);
void OLED_pos(uint8_t row, uint8_t column);volatile void OLED_write_data(char);void OLED_write_command(char command);void OLED_goto_row(uint8_t row);void OLED_goto_column(uint8_t column);void OLED_reset(void);void OLED_home(void);void OLED_clear_line(uint8_t line);void OLED_print(char* data);void OLED_set_brightness(uint8_t lvl);
#endif