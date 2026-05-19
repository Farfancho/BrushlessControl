/* 
 * File:   task.h
 * Author: alejandro
 *
 * Created on May 17, 2026, 4:42 PM
 */

#ifndef TASK_H
#define	TASK_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "sensors.h"
#include "serial.h"
#include "control.h"
#include "motor.h"
#include "mcc_generated_files/mcc.h"

    void taskInit();
    void AppTask();
   
#ifdef	__cplusplus
}
#endif

#endif	/* TASK_H */

