/* 
 * File:   task.h
 * Author: alejandro
 *
 * Created on May 8, 2026, 10:10 PM
 */

#ifndef TASK_H
#define	TASK_H

#ifdef	__cplusplus
extern "C" 
#endif
    
typedef enum
{
    SETPOINT_SERIAL = 0,
    SETPOINT_ANALOG = 1
} setpoint_mode_t;

extern volatile setpoint_mode_t setpoint_mode;

void App_Init(void);
void App_ControlTask(void);
void App_BackgroundTask(void);

    
#endif	/* TASK_H */

