#ifndef MOTOR_H
#define MOTOR_H


#ifdef __cplusplus
extern "C" 
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    MOTOR_DIR_CW = 0,
    MOTOR_DIR_CCW = 1
} motor_dir_t;

typedef enum{
    MV_ABSOLUTE = 0,
    MV_RELATIVE
}movement_type_t;

typedef struct {
    int32_t position;
    int32_t target;
    motor_dir_t direction;
    bool moving;
    movement_type_t movementType;
    uint16_t dutyCycle;//este valor debe ser flotante (?) 
}motor_status_t;

void Motor_init(volatile motor_status_t *motor,
    int32_t position,
    int32_t target,
    motor_dir_t direction,
    bool moving,
    movement_type_t movement_type,
    uint16_t dutyCycle
);



#endif /* MOTOR_H */