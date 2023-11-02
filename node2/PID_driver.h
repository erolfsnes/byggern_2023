#ifndef PID_DRIVER_H_
#define PID_DRIVER_H_

typedef struct{
	float K_p;
	float K_i;
	float K_d;
	float T;
	float integral_sum;
	float prev_error;
} PID_parameters;

void PID_init(float K_p, float K_i, float K_d, float T);
float PID_controller(int16_t target_val, int16_t current_val);
uint16_t PID_scale(int8_t pos);

#endif