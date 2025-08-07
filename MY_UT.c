#include "MY_UT.h"




void MY_Delay_us(int temp)
{
	volatile int delay_us_temp = temp*5;
	while(delay_us_temp--);
}

float MY_HPF(float input, float* prev_in, float* prev_out)
{
    float output = alpha * (*prev_out + input - *prev_in);
    *prev_in = input;
    *prev_out = output;
    return output;
}
