#include "PWM_driver.h"
#include "sam.h"
#include "printf-stdarg.h"
#include <stdint.h>
#include <stdlib.h>
#include "PID_driver.h"

static PID_parameters parameters = {0};

void PID_init(float K_p, float K_i, float K_d, float T)
{
	parameters.K_p = K_p;
	parameters.K_i = K_i;
	parameters.K_d = K_d;
	parameters.T = T;
	
	parameters.prev_error = 0;
	parameters.integral_sum = 0;
}

float PID_controller(int16_t target_val, int16_t current_val)
{
	int16_t error = target_val - current_val;
	
	// Proportional part of controller
	float u_prop = error * parameters.K_p;
	
	// Integral part of controller
	parameters.integral_sum += parameters.K_i * error;
	float u_integ = parameters.T * parameters.integral_sum;
	
	// Derivative part of controller
	float u_der = (parameters.K_d / parameters.T)*(error - parameters.prev_error);
	
	// Control output
	float u = u_prop + u_integ;// + u_der;
	
	// Storing the current error to be used as previous error in next iteration
	parameters.prev_error = error;
	
	return u;
}

uint16_t PID_scale(int8_t pos)
{
	uint16_t scaled_value = (((float)pos+100)/(100+100))*(1390-10) + 10;
	return (uint16_t)scaled_value;
}