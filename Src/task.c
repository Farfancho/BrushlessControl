#include "task.h"

void taskInit(){
    sensorsInit(sensor, 0, 0);
    controlInit(ctrl,
            0,                  // setPoint
            0,                  // error
            0,                  // prevError
            0,                  // output
            0,                  //integral
            200,                 // kp
            0,                  // ki
            0,                  // kd
            SETPOINT_ANALOG);   // mode
    motorInit();
}

static void serialTX (){
//    if(serial_send_flag){
//        serial_send_flag = false;
//        int16_t AnSetpointTX = ADCC_GetSingleConversion(AnSetpoint);
//        int16_t PositionTX = ADCC_GetSingleConversion(Position);
//        int16_t ErrorTX = AnSetpointTX - PositionTX;
//        updateAnSetpoint(sensor, AnSetpointTX);
//        updatePosition(sensor, PositionTX);
        //SerialSendPlot4(AnSetpointTX,PositionTX,ErrorTX + 4095,0);
}


void AppTask(){
    Serial_Task();
    
    if(controlTickFlag){
        //DIR_Toggle(); //debug
        controlTickFlag = false;
        
        uint16_t AnSetpointraw;
        uint16_t position;
        
        AnSetpointraw = ADCC_GetSingleConversion(AnSetpoint);
        position = ADCC_GetSingleConversion(Position);
        
        updateAnSetpoint(sensor, AnSetpointraw);
        updatePosition(sensor, position);
        
        if(getMode(ctrl) == SETPOINT_ANALOG){ //replace with the getter
            updateSetPoint(ctrl, getAnSetpoint(sensor));
        }
        //else{}//setpoint digital 
        
        
        updateError(ctrl, getPosition(sensor));
        updatePIDOutput(ctrl);  
        
        motorApplyOutput(getOutput(ctrl));
        
        SerialSendPlot4(
            (uint16_t)getSetpoint(ctrl),
            (uint16_t)getPosition(sensor),
            (uint16_t)(getError(ctrl) + 4095),
            (uint16_t)(getOutput(ctrl) + 4095)
        );
    }
}