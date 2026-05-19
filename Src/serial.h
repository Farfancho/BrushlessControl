/* 
 * File:   serial.h
 * Author: alejandro
 *
 * Created on May 17, 2026, 1:31 PM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc_generated_files/eusart1.h" //maxbaudrate
#include "control.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
    
typedef enum {
    CMD_NONE,
    CMD_i,//integral constant
    CMD_p,//proportional constant
    CMD_d,//derivative constant
    CMD_M,//Move to
    CMD_A,//Analog input
} CommandID;

uint16_t decodeGcode(char *input);
void SerialWriteByte(uint8_t byte);
void SerialSendPlot4(uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
void Serial_Task(void);
void Serial_ProcessCommand(char *input);




#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_H */

