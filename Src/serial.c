#include <xc.h>
#include "serial.h"


char rx_buffer[32];
uint8_t rx_index = 0;

uint16_t decodeGcode(char *input){
    if (strncmp(input, "i", 1) == 0) return CMD_i; //integral constant
    if (strncmp(input, "p", 1) == 0) return CMD_p; //proportional constant
    if (strncmp(input, "d", 1) == 0) return CMD_d; //derivative constant
    if (strncmp(input, "M", 1) == 0) return CMD_M; //move to
    if (strncmp(input, "A", 1) == 0) return CMD_A; //Analog input
    return CMD_NONE;
}


/*
 * Previous way to send data
//void SerialSendData(int16_t value){
//    while (!EUSART1_is_tx_ready());
//    EUSART1_Write((uint8_t)(value & 0xFF));
//
//    while (!EUSART1_is_tx_ready());
//    EUSART1_Write((uint8_t)((value >> 8) & 0xFF));
//}
*/


void SerialWriteByte(uint8_t byte){
    while (!EUSART1_is_tx_ready());
    EUSART1_Write(byte);
}

void SerialSendPlot4(uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4){
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
    //value = atoi(&input[1]); to get the info after the decodeGcode

    cmd = decodeGcode(input);

    switch (cmd)
    {
        case CMD_i:
            //change the integral constant
            value = atoi(&input[1]);
            
            if(value < 0){
                value = 0;             
            }else if(value > 1000){
                value = 1000;
            }
            
            updateKi(ctrl, (uint16_t)value);
            break;

        case CMD_p:
            //change the proportional constant
            value = atoi(&input[1]);
            
            if(value < 0){
                value = 0;             
            }else if(value > 1000){
                value = 1000;
            }
            
            updateKp(ctrl, (uint16_t)value);
            break;
        case CMD_d:
            //change the derivative constant
            value = atoi(&input[1]);
            
            if(value < 0){
                value = 0;             
            }else if(value > 1000){
                value = 1000;
            }
            
            updateKd(ctrl, (uint16_t)value);
            break;
        case CMD_M:
            value = atoi(&input[1]);
            
            if(value < 0){
                
                value = 0;                
            }else if (value > SENSOR_ADC_MAX){
                
                value = SENSOR_ADC_MAX;
            }
            
            updateSetPoint(ctrl, (int16_t)value);
            updateMode(ctrl, SETPOINT_SERIAL);
            //Serial input
            break;
        case CMD_A:
            updateMode(ctrl, SETPOINT_ANALOG);
            //Set input to Analog
            break;
        case CMD_NONE:
        default:
            break;
    }
}
