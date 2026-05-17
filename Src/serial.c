#include <xc.h>
#include "serial.h"
#include "control.h"
#include "task.h"
#include <stdlib.h>
//configurar baudrate de la uart

char rx_buffer[32];
uint8_t rx_index = 0;

uint16_t decodeGcode(char *input) {
    if (strncmp(input, "i", 1) == 0) return CMD_i; //integral constant
    if (strncmp(input, "p", 1) == 0) return CMD_p; //proportional constant
    if (strncmp(input, "d", 1) == 0) return CMD_d; //derivative constant
    if (strncmp(input, "M", 1) == 0) return CMD_M; //move to
    if (strncmp(input, "A", 1) == 0) return CMD_A; //Analog input
    if (strncmp(input, "S", 1) == 0) return CMD_S; //Serial Setpoint
    return CMD_NONE;
}

void SerialSendData(int16_t value){//maybe unused, but it derives to the SerialSendPlot4
    while (!EUSART1_is_tx_ready());
    EUSART1_Write((uint8_t)(value & 0xFF));

    while (!EUSART1_is_tx_ready());
    EUSART1_Write((uint8_t)((value >> 8) & 0xFF));
}


void SerialWriteByte(uint8_t byte)
{
    while (!EUSART1_is_tx_ready());
    EUSART1_Write(byte);
}

void SerialSendPlot4(uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4)
{
    SerialWriteByte((uint8_t)(v1 & 0xFF));
    SerialWriteByte((uint8_t)((v1 >> 8) & 0xFF));

    SerialWriteByte((uint8_t)(v2 & 0xFF));
    SerialWriteByte((uint8_t)((v2 >> 8) & 0xFF));

    SerialWriteByte((uint8_t)(v3 & 0xFF));
    SerialWriteByte((uint8_t)((v3 >> 8) & 0xFF));

    SerialWriteByte((uint8_t)(v4 & 0xFF));
    SerialWriteByte((uint8_t)((v4 >> 8) & 0xFF));
}

void Serial_Task(void)
{
    char c;

    if (PIR3bits.RC1IF) //si hay dato
    {
        c = EUSART1_Read();

        if (c == '\n')
        {
            rx_buffer[rx_index] = '\0';
            Serial_ProcessCommand(rx_buffer);
            rx_index = 0;
        }
        else if (rx_index < sizeof(rx_buffer) - 1)
        {
            rx_buffer[rx_index++] = c;
        }
    }
}

void Serial_ProcessCommand(char *input)
{
    int16_t cmd;
    int32_t value = 0;
    //value = atoi(&input[1]); to get the info after the decodegcode

    cmd = decodeGcode(input);

    switch (cmd)
    {
        case CMD_i:
            //change the integral constant
            Control_SetKi(atoi(&input[1]));
            break;

        case CMD_p:
            //change the proportional constant
            Control_SetKp(atoi(&input[1]));
            break;
        case CMD_d:
            //change the derivative constant
            Control_SetKd(atoi(&input[1]));
            break;
        case CMD_M:
            //change setpoint via Serial input
            setpoint_mode = SETPOINT_SERIAL;
            motor->target = atoi(&input[1]);
            break;
        case CMD_S:
            //emergency Stop maybe not used 
            break;
        case CMD_A:
            //Toggle Input type
            setpoint_mode = SETPOINT_ANALOG;
            break;
        case CMD_NONE:
        default:
            break;
    }
}
