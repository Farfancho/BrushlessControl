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
