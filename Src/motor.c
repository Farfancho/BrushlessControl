#include "motor.h"
#include "serial.h"


void motorInit (){
    PWM6_LoadDutyValue(0);
}


void motorApplyOutput(int16_t output){
    
    uint16_t dutyCycle;
    int16_t absOutput;
    
    if(output > MOTOR_OUTPUT_LIMIT){
        output = MOTOR_OUTPUT_LIMIT;
    }
    else if (output < -MOTOR_OUTPUT_LIMIT){
        output = -MOTOR_OUTPUT_LIMIT;
    }
    
    
    if (output > 0){
        DIR_SetLow();
        absOutput = output;
    }else{
        DIR_SetHigh();
        absOutput = -output;
    }
    
    dutyCycle = (uint16_t)(((uint32_t)absOutput * MOTOR_MAX_PWM)/ MOTOR_CONTROL_MAX); //replace for bitshift
    
    PWM6_LoadDutyValue(dutyCycle);
    
}
