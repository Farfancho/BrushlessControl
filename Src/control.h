#ifndef CONTROL_H
#define CONTROL_H

#ifdef __cplusplus
extern "C" 
#endif

#include <stdint.h>
#include <stdbool.h>
#include "motor.h"

typedef struct {
    uint16_t Ki;//hay que escalarlos x1000 maybe
    uint16_t Kp;
    uint16_t Kd;
    int32_t Error;
    int32_t prevError;
    int32_t Ts;
}params_t;

typedef struct {
    int32_t integral; //int32_t
    int32_t derivative;
    int32_t proportional;
}components_t;

void params_init(volatile params_t *params,
    uint16_t Ki,
    uint16_t Kp,
    uint16_t Kd,
    int32_t Error,
    int32_t prevError,
    int32_t Ts);

void components_init(volatile components_t *components,
    int32_t integral,
    int32_t derivative,
    int32_t proportional);

int32_t Motor_GetError(volatile params_t *params);
void Motor_SetError(volatile motor_status_t *motor, volatile params_t *params);
void SetIntegralComponent(params_t *params, components_t *components);
void SetDerivativeComponent(params_t *params, components_t *components);
void SetProportionalComponent(params_t *params, components_t *components);
float GetPIDOutput(components_t *components);

#endif /* CONTROL_H */
/*

typedef struct {
    float Kp;
    float Ki;
    float Kd;

    float Ts;

    float e_k;
    float e_k_1;
    float e_k_2;

    float u_k;
    float u_k_1;

    float out_min;
    float out_max;
} PID_t;


para el control se pueden usar ambos casos, pero para aplicaciones de control digital es util:
ecuacion en diferencias

u[k-1]
e[k]
e[k-1]
e[k-2]

u[k] = u[k-1] + q0 ​ek​ + q1 ek-1 + q2 ek - 2;

float PID_Update(PID_t *pid, float setpoint, float position)
{
    pid->e_k = setpoint - position;

    float q0 = pid->Kp + pid->Ki * pid->Ts + pid->Kd / pid->Ts;
    float q1 = -pid->Kp - 2.0f * pid->Kd / pid->Ts;
    float q2 = pid->Kd / pid->Ts;

    pid->u_k = pid->u_k_1
             + q0 * pid->e_k
             + q1 * pid->e_k_1
             + q2 * pid->e_k_2;

    if(pid->u_k > pid->out_max) pid->u_k = pid->out_max;
    if(pid->u_k < pid->out_min) pid->u_k = pid->out_min;

    pid->e_k_2 = pid->e_k_1;
    pid->e_k_1 = pid->e_k;
    pid->u_k_1 = pid->u_k;

    return pid->u_k;
}
*/