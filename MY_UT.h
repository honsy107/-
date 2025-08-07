#ifndef __MY_UT_H__
#define __MY_UT_H__

static float prev_red_in = 0, prev_red_out = 0;
static float prev_ir_in = 0, prev_ir_out = 0;
static float alpha = 0.95f;

void MY_Delay_us(int temp);
float MY_HPF(float input, float* prev_in, float* prev_out);

#endif
