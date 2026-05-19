#include "control.h"


#define SAMPLING_RATE 150U
#define CONTROL_GAIN_SCALE 100L //escalarlo para no usar float
#define CONTROL_OUTPUT_MAX 4095
#define CONTROL_INTEGRAL_MAX 20000L

volatile control_status_t control_data;
volatile control_status_t *ctrl = &control_data;

void controlInit(volatile control_status_t *ctrl, 
        int16_t setPoint,
        int16_t error, 
        int16_t prevError, 
        int16_t output, 
        int32_t integral,
        uint16_t kp, 
        uint16_t ki, 
        uint16_t kd,
        setpoint_mode_t mode){
    ctrl->setPoint = setPoint;
    ctrl->error = error;
    ctrl->prevError = prevError;
    ctrl->output = output;
    ctrl->integral = integral;
    ctrl->kp = kp;
    ctrl->ki = ki;
    ctrl->kd = kd;
    ctrl->mode = mode;
}

void discretizeKi(volatile control_status_t *ctrl){ 
    ctrl->ki = ctrl->ki / SAMPLING_RATE; //replace later for a bit shift
}

void discretizeKd(volatile control_status_t *ctrl){
    ctrl->kd = ctrl->kd * SAMPLING_RATE; //replace later for a bit shift
}

void updateSetPoint(volatile control_status_t *ctrl, int16_t setPoint){
    if(setPoint < 0) {setPoint = 0;}
    if(setPoint > SENSOR_ADC_MAX) {setPoint = SENSOR_ADC_MAX;}
    ctrl->setPoint = setPoint;
}

int16_t getSetpoint(volatile control_status_t *ctrl){
    return ctrl->setPoint;
}

void updateError(volatile control_status_t *ctrl, int16_t position){
    ctrl->prevError = ctrl->error;
    ctrl->error = ctrl->setPoint - position;
}

int16_t getError(volatile control_status_t *ctrl){
    return ctrl->error;
}

//void updatePOutput(volatile control_status_t *ctrl){
//    int32_t output;
//    output = ((int32_t)ctrl->error * (int32_t)ctrl->kp)/CONTROL_KP_SCALE;//replace later for a bit shift
//    
//    if (output > CONTROL_OUTPUT_MAX){
//        output = CONTROL_OUTPUT_MAX;
//    }else if (output < -CONTROL_OUTPUT_MAX){
//        output = -CONTROL_OUTPUT_MAX;
//    }
//    
//    ctrl->output = (int16_t)output;    
//}

void updatePIDOutput(volatile control_status_t *ctrl){
    int32_t pTerm;
    int32_t iTerm;
    int32_t dTerm;
    int32_t output;
    int16_t derivative;
    
    ctrl->integral += ctrl->error;
    
    if (ctrl->integral > CONTROL_INTEGRAL_MAX)
    {
        ctrl->integral = CONTROL_INTEGRAL_MAX;
    }
    else if (ctrl->integral < -CONTROL_INTEGRAL_MAX)
    {
        ctrl->integral = -CONTROL_INTEGRAL_MAX;
    }
    
    derivative = ctrl->error - ctrl->prevError;
    
    pTerm = (int32_t)ctrl->kp * (int32_t)ctrl->error;
    iTerm = (int32_t)ctrl->ki * ctrl->integral;
    dTerm = (int32_t)ctrl->kd * (int32_t)derivative;

    output = (pTerm + iTerm + dTerm) / CONTROL_GAIN_SCALE;

    if (output > CONTROL_OUTPUT_MAX)
    {
        output = CONTROL_OUTPUT_MAX;
    }
    else if (output < -CONTROL_OUTPUT_MAX)
    {
        output = -CONTROL_OUTPUT_MAX;
    }

    ctrl->output = (int16_t)output; 
    
}

int16_t getOutput(volatile control_status_t *ctrl){
    return ctrl->output;
}

void updateKp (volatile control_status_t *ctrl, uint16_t kp){
    ctrl->kp = kp;
}

uint16_t getKp(volatile control_status_t *ctrl){
    return ctrl->kp;
} 

void updateMode(volatile control_status_t *ctrl, setpoint_mode_t mode){
    ctrl->mode = mode;
}
setpoint_mode_t getMode(volatile control_status_t *ctrl){
    return ctrl->mode;
}

void updateKi(volatile control_status_t *ctrl, uint16_t ki){
    ctrl->ki = ki;
}
uint16_t getKi(volatile control_status_t *ctrl){
    return ctrl->ki;
}

void updateKd(volatile control_status_t *ctrl, uint16_t kd){
    ctrl->kd = kd;
}
uint16_t getKd(volatile control_status_t *ctrl){
    return ctrl->kd;
}

void resetIntegral(volatile control_status_t *ctrl){
    ctrl->integral = 0;
}