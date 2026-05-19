/* 
 * File:   control.h
 * Author: alejandro
 *
 * Created on May 17, 2026, 4:42 PM
 */

#ifndef CONTROL_H
#define	CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "sensors.h"

    
typedef enum
{
    SETPOINT_ANALOG = 0,
    SETPOINT_SERIAL
} setpoint_mode_t;

typedef struct
{
    int16_t setPoint;
    int16_t error;
    int16_t prevError;
    int16_t output;
    
    int32_t integral;

    uint16_t kp;
    uint16_t ki;
    uint16_t kd;

    setpoint_mode_t mode;
} control_status_t;

extern volatile control_status_t control_data;
extern volatile control_status_t *ctrl;

void controlInit(volatile control_status_t *ctrl, 
        int16_t setPoint,
        int16_t error,
        int16_t prevError,
        int16_t output,
        int32_t integral,
        uint16_t kp,
        uint16_t ki,
        uint16_t kd,
        setpoint_mode_t mode);


//this routines will be called on the serial module to avoid multiplying on the PID runtime
void discretizeKi(volatile control_status_t *ctrl); //updates Ki to ki/Ts
void discretizeKd(volatile control_status_t *ctrl); //updates Kd to Kd*Ts

//setters and getters for the structure variables

void updateError(volatile control_status_t *ctrl, int16_t position);
int16_t getError(volatile control_status_t *ctrl);

void updateSetPoint(volatile control_status_t *ctrl, int16_t setPoint);
int16_t getSetpoint(volatile control_status_t *ctrl);

void updatePIDOutput(volatile control_status_t *ctrl);
int16_t getOutput(volatile control_status_t *ctrl);

void updateKp(volatile control_status_t *ctrl, uint16_t kp);
uint16_t getKp(volatile control_status_t *ctrl);

void updateKi(volatile control_status_t *ctrl, uint16_t ki);
uint16_t getKi(volatile control_status_t *ctrl);

void updateKd(volatile control_status_t *ctrl, uint16_t kd);
uint16_t getKd(volatile control_status_t *ctrl);

void updateMode(volatile control_status_t *ctrl, setpoint_mode_t mode);
setpoint_mode_t getMode(volatile control_status_t *ctrl);



#ifdef	__cplusplus
}
#endif

#endif	/* CONTROL_H */

