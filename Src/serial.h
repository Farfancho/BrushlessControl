#ifndef SERIAL_H
#define SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
    
typedef enum {
    CMD_NONE,
    CMD_i,//integral constant
    CMD_p,//proportional constant
    CMD_d,//derivative constant
    CMD_M,//Move to
    CMD_S,//Serial Setpoint
    CMD_A,//Analog input
} CommandID;

void Serial_Task(void);
void Serial_Timer100msISR(void);
uint16_t decodeGcode(char *input);
void SerialSendData(int16_t value);
void Serial_ProcessCommand(char *input);

void SerialSendPlot4(uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
static void SerialWriteByte(uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_H */
