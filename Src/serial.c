#include "serial.h"

uint16_t decodeGcode(char *input) {
    if (strncmp(input, "i", 1) == 0) return CMD_i; //integral constant
    if (strncmp(input, "p", 1) == 0) return CMD_p; //proportional constant
    if (strncmp(input, "d", 1) == 0) return CMD_d; //derivative constant
    if (strncmp(input, "M", 1) == 0) return CMD_M; //move to
    if (strncmp(input, "A", 1) == 0) return CMD_A; //Analog input
    if (strncmp(input, "S", 1) == 0) return CMD_S; //Serial Setpoint
    return CMD_NONE;
}
