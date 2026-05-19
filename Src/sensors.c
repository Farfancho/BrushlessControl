#include <xc.h>
#include "sensors.h"

volatile sensor_status_t sensor_data;
volatile sensor_status_t *sensor = &sensor_data;

void sensorsInit (volatile sensor_status_t *sensor, uint16_t AnSetpoint, uint16_t Position){
    sensor->AnSetpoint = AnSetpoint;
    sensor->Position = Position;
}

void updateAnSetpoint(volatile sensor_status_t* sensor, uint16_t AnSetpoint){
    sensor->AnSetpoint = AnSetpoint;
}

void updatePosition(volatile sensor_status_t* sensor, uint16_t position){
    sensor->Position = position;
}

uint16_t getAnSetpoint(volatile sensor_status_t* sensor){
    return sensor->AnSetpoint;
}

uint16_t getPosition(volatile sensor_status_t* sensor){
    return sensor->Position;
}
