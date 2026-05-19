/* 
 * File:   sensors.h
 * Author: alejandro
 *
 * Created on May 17, 2026, 1:33 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include "mcc_generated_files/adcc.h"

#define SENSOR_ADC_MAX          4095UL //ADC resolution

typedef struct
{
    uint16_t AnSetpoint;
    uint16_t Position;
} sensor_status_t;

extern volatile sensor_status_t sensor_data;
extern volatile sensor_status_t *sensor;

void sensorsInit(volatile sensor_status_t *sensor, uint16_t AnSetpoint, uint16_t Position); //initialize
void updateAnSetpoint(volatile sensor_status_t *sensor, uint16_t AnSetpoint); 
void updatePosition(volatile sensor_status_t *sensor, uint16_t AnSetpoint);
uint16_t getAnSetpoint(volatile sensor_status_t *sensor);
uint16_t getPosition(volatile sensor_status_t *sensor); 

#ifdef	__cplusplus
}
#endif

#endif	/* SENSORS_H */

