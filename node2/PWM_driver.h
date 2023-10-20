#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_


void PWM_init(void);
void PWM_set_duty_cycle(float duty_cycle);

#define PWM_FREQ 84000000/128.0
#define PWM_PERIOD 20.0/1000


#endif