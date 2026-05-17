/* 
 * File:   sensors.h
 * Author: alejandro
 *
 * Created on May 8, 2026, 10:10 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H


#ifdef __cplusplus
extern "C" 
#endif


#include <stdint.h>
#include <stdbool.h>
#include "../mcc_generated_files/adcc.h"

typedef struct {
    uint16_t feedback;
    uint16_t analogInput;    
}sensors_status_t;

extern volatile sensors_status_t sensor_data;
extern volatile sensors_status_t *sensor;

void sensorsInit(volatile sensors_status_t *sensor,
                 uint16_t feedback,
                 uint16_t analogInput);

void setFeedback (adcc_channel_t channel, volatile sensors_status_t *sensor);
void setAnalogInput (adcc_channel_t channel, volatile sensors_status_t *sensor);
uint16_t getFeedback (volatile sensors_status_t *sensor);
uint16_t getAnalogInput (volatile sensors_status_t *sensor);

#endif	/* SENSORS_H */

/**
 End of File
*/