#ifndef MOTOR_H
#define MOTOR_H


#ifdef __cplusplus
extern "C" 
#endif

#include <stdint.h>
#include <stdbool.h>
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/pwm6.h"

typedef enum
{
    MOTOR_DIR_CW = 0,
    MOTOR_DIR_CCW = 1
} motor_dir_t;

typedef struct {
    uint16_t position;
    uint16_t target;
    motor_dir_t direction;
    uint16_t dutyCycle;//este valor debe ser flotante (?) 
}motor_status_t;

void Motor_init(volatile motor_status_t *motor,
    uint16_t position,
    uint16_t target,
    motor_dir_t direction,
    uint16_t dutyCycle
);
extern volatile motor_status_t motor_data;
extern volatile motor_status_t *motor;

void Motor_ApplyOutput(int32_t output);

#endif /* MOTOR_H */

/**
 End of File
*/