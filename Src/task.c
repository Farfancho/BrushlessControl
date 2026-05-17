//this is the parent header, it should be able to access all the other headers
#include <xc.h>
#include <stdbool.h>
#include <string.h>
#include "task.h"

#include "motor.h"
#include "control.h"
#include "sensors.h"
#include "serial.h"

#include "../mcc_generated_files/tmr1.h"

#define TELEMETRY_DECIMATION 15   // 150 Hz / 15 = 10 Hz aprox.

volatile uint8_t telemetry_counter = 0;
volatile bool telemetry_ready = false;

volatile uint16_t telemetry_position = 0;
volatile uint16_t telemetry_target = 0;
volatile int16_t telemetry_error = 0;
volatile int16_t telemetry_output = 0;

void App_Init(void)
{
    Motor_init(motor, 0, 2000, MOTOR_DIR_CW, 0);

    params_init(params,
                0,      // Ki
                10,     // Kp
                0,      // Kd
                0,      // Error
                0,      // prevError
                6667);  // Ts en us para 150 Hz

    components_init(components, 0, 0, 0);

    sensorsInit(sensor, 0, 0);

    TMR1_SetInterruptHandler(App_ControlTask);
}

volatile setpoint_mode_t setpoint_mode = SETPOINT_SERIAL;

void App_ControlTask(void)
{
    int32_t output;

    // 1. Leer sensor
    setFeedback(SetPoint, sensor);

    // 2. Actualizar posición
    motor->position = getFeedback(sensor);
    
    switch (setpoint_mode)
    {
        case SETPOINT_SERIAL:
            // El target ya fue actualizado por el comando Mxxxx
            break;

        case SETPOINT_ANALOG:
            setAnalogInput(AnalogIN, sensor);
            motor->target = getAnalogInput(sensor);
            break;

        default:
            break;
    }

    // 3. Calcular error
    Motor_SetError(motor, params);

    // 4. Calcular PID
    SetProportionalComponent(params, components);
    SetIntegralComponent(params, components);
    SetDerivativeComponent(params, components);

    output = GetPIDOutput(components);

    // 5. Actualizar error anterior
    params->prevError = params->Error;

    // 6. Aplicar al motor
    Motor_ApplyOutput(output);
    
    // 8. Guardar telemetría
    telemetry_position = motor->position;
    telemetry_target = motor->target;
    telemetry_error = (int16_t)params->Error;
    telemetry_output = (int16_t)motor->dutyCycle;

    // 9. Enviar solo cada cierto número de ciclos
    telemetry_counter++;

    if (telemetry_counter >= TELEMETRY_DECIMATION)
    {
        telemetry_counter = 0;
        telemetry_ready = true;
    }
}

void App_BackgroundTask(void)
{
    uint16_t pos;
    uint16_t target;
    int16_t error;
    int16_t output;

    Serial_Task();

    if (telemetry_ready)
    {


        telemetry_ready = false;

        pos = telemetry_position;
        target = telemetry_target;
        error = telemetry_error;
        output = telemetry_output;

        SerialSendPlot4(pos, target, error, output);
    }
}