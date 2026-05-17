#include <xc.h> 

#include "motor.h"

#define PWM_MAX_DUTY 1023

void Motor_init(volatile motor_status_t *motor,
        uint16_t position,
        uint16_t target,
        motor_dir_t direction,
        uint16_t dutyCycle){
    motor->position = position;
    motor->target = target;
    motor->direction = direction;
    motor->dutyCycle = dutyCycle;
}

volatile motor_status_t motor_data; 
volatile motor_status_t *motor = &motor_data; 


void Motor_ApplyOutput(int32_t output)
{
    uint16_t duty;

    if (output > 0)
    {
        motor->direction = MOTOR_DIR_CW;
        duty = (uint16_t)output;
        DIR_SetHigh();
    }
    else if (output < 0)
    {
        motor->direction = MOTOR_DIR_CCW;
        duty = (uint16_t)(-output);
        DIR_SetLow();
    }
    else
    {
        duty = 0;
    }

    if (duty > PWM_MAX_DUTY)
    {
        duty = PWM_MAX_DUTY;
    }

    motor->dutyCycle = duty;
    PWM6_LoadDutyValue(duty);
}
