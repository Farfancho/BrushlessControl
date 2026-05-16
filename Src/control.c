#include <xc.h> 

#include "control.h"

//es posible que toque cambiar el enfoque a ecuaciones en diferencias

void params_init(volatile params_t *params,
        uint16_t Ki,
        uint16_t Kp,
        uint16_t Kd,
        int32_t Error,
        int32_t prevError,
        int32_t Ts){
    params->Kd = Kd;
    params->Ki = Ki;
    params->Kp = Kp;   
    params->Error = Error;
    params->prevError = prevError;
    params->Ts = Ts;

}

void components_init(volatile components_t *components,
        int32_t integral,
        int32_t derivative,
        int32_t proportional){
    components->integral = integral;
    components->derivative = derivative;
    components->proportional = proportional;
}

volatile params_t paramsData; 
volatile params_t *params = &paramsData; 

volatile components_t componentsData;
volatile components_t *components = &componentsData;

int32_t Motor_GetError(volatile params_t *params){ //serial
    return params->Error;
}

void Motor_SetError(volatile motor_status_t *motor, volatile params_t *params){
    params->Error = motor->target - motor->position;
}

void SetIntegralComponent(params_t *params, components_t *components){
    components->integral += params->Ki * params->Error * params->Ts;
    //creo que debo limitar el valor que puede tomar el componente integral para
    //evitar saturaci�n
}

void SetDerivativeComponent(params_t *params, components_t *components){
    components->derivative = params->Kd * ((params->Error - params->prevError) / params->Ts);
}

void SetProportionalComponent(params_t *params, components_t *components){
    components->proportional = params->Kp * params->Error;
}

float GetPIDOutput(components_t *components){
    return components->proportional + components->integral + components->derivative;
}

/*
en las llamadas 
pid_output = GetPIDOutput(&components);
params.prevError = params.Error;
*/
