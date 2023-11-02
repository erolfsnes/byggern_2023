#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

void motor_init(void);
void motor_write(int8_t input, uint8_t button);
int16_t motor_read(void);
void delay_us(uint32_t microseconds);
uint16_t scale_to_motor(int8_t input);

#endif