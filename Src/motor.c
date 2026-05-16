#include <xc.h> 

#include "motor.h"

void Motor_init(volatile motor_status_t *motor,
        int32_t position,
        int32_t target,
        motor_dir_t direction,
        uint16_t dutyCycle){
    motor->position = position;
    motor->target = target;
    motor->direction = direction;
    motor->dutyCycle = dutyCycle;
}

volatile motor_status_t motor_data; 
volatile motor_status_t *motor = &motor_data; 
