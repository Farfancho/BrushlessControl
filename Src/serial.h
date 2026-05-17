#ifndef SERIAL_H
#define SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../mcc_generated_files/eusart1.h"
#include "motor.h"
    
typedef enum {
    CMD_NONE,
    CMD_i,//integral constant
    CMD_p,//proportional constant
    CMD_d,//derivative constant
    CMD_M,//Move to
    CMD_S,//Stop
    CMD_A,//Analog input
} CommandID;

void Serial_Task(void);
void Serial_Timer100msISR(void);
uint16_t decodeGcode(char *input);
void SerialSendData(int16_t value);
void Serial_ProcessCommand(char *input);

void SerialSendPlot4(uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
void SerialWriteByte(uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_H */

/**
 End of File
*/