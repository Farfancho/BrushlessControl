/* 
 * File:   motor.h
 * Author: alejandro
 *
 * Created on May 18, 2026, 11:39 AM
 */

#ifndef MOTOR_H
#define	MOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "mcc_generated_files/pwm6.h"
#include "mcc_generated_files/pin_manager.h"
    
#define MOTOR_MAX_PWM 1023U //max real output (unsigned for dir)
#define MOTOR_CONTROL_MAX 4095 // max logic output
#define MOTOR_OUTPUT_LIMIT 1024 //debug for safety
#define MOTOR_DEADBAND 20 //deadzone
    
    void motorInit();
    void motorApplyOutput(int16_t output);



#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_H */

