#ifndef __PID_H__
#define __PID_H__

#include <stdint.h>
#include <stdbool.h> // 包含 stdbool.h 以定义 bool 类型

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float setpoint;
    float output_min;
    float output_max;
} PID_Params;

typedef struct {
    PID_Params params;
    float input;
    float output;
    float integral;
    float last_error;
    bool initialized;
} PID_HandleTypeDef;

void PID_Init(PID_HandleTypeDef *hp, const PID_Params *params);
float PID_Compute(PID_HandleTypeDef *hp, float input);

#endif /* __PID_H__ */

