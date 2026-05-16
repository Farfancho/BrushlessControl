//debe contener la medición de cada uno de los ADCs

#include <xc.h>
#include "sensors.h"

void sensorsInit(volatile sensors_status_t *sensor,
uint16_t feedback,
uint16_t analogInput){
    sensor->feedback = feedback;
    sensor->analogInput = analogInput;
}

volatile sensors_status_t sensor_data;
volatile sensors_status_t *sensor = &sensor_data;

void setFeedback (adcc_channel_t channel, volatile sensors_status_t *sensor){
    sensor->feedback = ADCC_GetSingleConversion(SetPoint); //adcc single convertion
}

uint16_t getFeedback (volatile sensors_status_t *sensor){
    return sensor->feedback;
}

void setAnalogInput(adcc_channel_t channel, volatile sensors_status_t *sensor){
    sensor->analogInput = ADCC_GetSingleConversion(AnalogIN); //adcc single convertion
}

uint16_t getAnalogInput (volatile sensors_status_t *sensor){
    return sensor->analogInput;
}