#include "pid.h"

void PID_Init(PID_HandleTypeDef *hp, const PID_Params *params) {
    hp->params.Kp = params->Kp;
    hp->params.Ki = params->Ki;
    hp->params.Kd = params->Kd;
    hp->params.setpoint = params->setpoint;
    hp->params.output_min = params->output_min;
    hp->params.output_max = params->output_max;

    hp->input = 0.0f;
    hp->output = 0.0f;
    hp->integral = 0.0f;
    hp->last_error = 0.0f;
    hp->initialized = true; // 确保 true 已定义
}

float PID_Compute(PID_HandleTypeDef *hp, float input) {
    if (!hp->initialized) {
        return 0.0f;
    }

    float error = hp->params.setpoint - input;
    hp->integral += error;
    float derivative = error - hp->last_error;

    hp->output = hp->params.Kp * error + hp->params.Ki * hp->integral + hp->params.Kd * derivative;

    if (hp->output > hp->params.output_max) {
        hp->output = hp->params.output_max;
    } else if (hp->output < hp->params.output_min) {
        hp->output = hp->params.output_min;
    }

    hp->last_error = error;

    return hp->output;
}
