#include <xc.h>
#include <stdint.h>

#include "control.h"

/*
 * CONTROL.C
 *
 * Convención usada:
 *  - Las ganancias que llegan por serial están escaladas x1000.
 *
 *    Ejemplos:
 *      p1000 -> Kp = 1.000
 *      i20   -> Ki = 0.020
 *      d80   -> Kd = 0.080
 *
 *  - params->Ts está en microsegundos.
 *    Para 150 Hz: Ts = 6667 us aproximadamente.
 *
 *  - Internamente se guardan:
 *      Kp_q      = Kp * 1000
 *      KiTs_q    = Ki * Ts * 1000
 *      KdDivTs_q = (Kd / Ts) * 1000
 *
 *  Así el ciclo de control no divide entre Ts ni multiplica por Ts.
 */

#ifndef GAIN_SCALE
#define GAIN_SCALE 1000L
#endif

//debug
//#ifndef CONTROL_OUTPUT_MAX
//#define CONTROL_OUTPUT_MAX 1023L
//#endif
//
//#ifndef CONTROL_OUTPUT_MIN
//#define CONTROL_OUTPUT_MIN (-1023L)
//#endif

#define CONTROL_OUTPUT_MAX 150L
#define CONTROL_OUTPUT_MIN (-150L)

#ifndef CONTROL_DEFAULT_TS_US
#define CONTROL_DEFAULT_TS_US 6667L
#endif

volatile params_t paramsData;
volatile params_t *params = &paramsData;

volatile components_t componentsData;
volatile components_t *components = &componentsData;

static int32_t clamp_i32(int32_t value, int32_t min, int32_t max)
{
    if (value > max)
    {
        return max;
    }

    if (value < min)
    {
        return min;
    }

    return value;
}

static int32_t safe_Ts_us(int32_t Ts_us)
{
    if (Ts_us <= 0)
    {
        return CONTROL_DEFAULT_TS_US;
    }

    return Ts_us;
}

static int32_t compute_KiTs_q(int32_t Ki_q, int32_t Ts_us)
{
    Ts_us = safe_Ts_us(Ts_us);

    /*
     * Ki_q representa Ki * 1000.
     * Ts_us / 1000000 convierte microsegundos a segundos.
     * Resultado queda todavía en escala x1000.
     */
    return (int32_t)(((int64_t)Ki_q * (int64_t)Ts_us) / 1000000LL);
}

static int32_t compute_KdDivTs_q(int32_t Kd_q, int32_t Ts_us)
{
    Ts_us = safe_Ts_us(Ts_us);

    /*
     * Kd_q representa Kd * 1000.
     * Dividir por Ts en segundos equivale a multiplicar por 1000000 / Ts_us.
     * Resultado queda todavía en escala x1000.
     */
    return (int32_t)(((int64_t)Kd_q * 1000000LL) / (int64_t)Ts_us);
}

void params_init(volatile params_t *p,
                 int32_t Ki_q,
                 int32_t Kp_q,
                 int32_t Kd_q,
                 int32_t Error,
                 int32_t prevError,
                 int32_t Ts_us)
{
    p->Ts = safe_Ts_us(Ts_us);
    p->Kp_q = Kp_q;
    p->KiTs_q = compute_KiTs_q(Ki_q, p->Ts);
    p->KdDivTs_q = compute_KdDivTs_q(Kd_q, p->Ts);
    p->Error = Error;
    p->prevError = prevError;
}

void components_init(volatile components_t *c,
                     int32_t integral,
                     int32_t derivative,
                     int32_t proportional)
{
    c->integral = integral;
    c->derivative = derivative;
    c->proportional = proportional;
}

void Control_SetKp(int32_t Kp_q)
{
    params->Kp_q = Kp_q;
}

void Control_SetKi(int32_t Ki_q)
{
    params->KiTs_q = compute_KiTs_q(Ki_q, params->Ts);
}

void Control_SetKd(int32_t Kd_q)
{
    params->KdDivTs_q = compute_KdDivTs_q(Kd_q, params->Ts);
}

void Control_SetTs(int32_t Ts_us)
{
    params->Ts = safe_Ts_us(Ts_us);
}

void Control_ResetIntegrator(void)
{
    components->integral = 0;
}

int32_t Motor_GetError(volatile params_t *p)
{
    return p->Error;
}

void Motor_SetError(volatile motor_status_t *m, volatile params_t *p)
{
    p->Error = m->target - m->position;
}

void SetProportionalComponent(volatile params_t *p, volatile components_t *c)
{
    c->proportional = p->Kp_q * p->Error;
}

void SetIntegralComponent(volatile params_t *p, volatile components_t *c)
{
    int32_t newIntegral;
    int32_t integralMax;
    int32_t integralMin;

    newIntegral = c->integral + (p->KiTs_q * p->Error);

    integralMax = CONTROL_OUTPUT_MAX * GAIN_SCALE;
    integralMin = CONTROL_OUTPUT_MIN * GAIN_SCALE;

    c->integral = clamp_i32(newIntegral, integralMin, integralMax);
}

void SetDerivativeComponent(volatile params_t *p, volatile components_t *c)
{
    c->derivative = p->KdDivTs_q * (p->Error - p->prevError);
}

int32_t GetPIDOutput(volatile components_t *c)
{
    int32_t output_q;
    int32_t output;

    output_q = c->proportional + c->integral + c->derivative;
    output = output_q / GAIN_SCALE;

    output = clamp_i32(output, CONTROL_OUTPUT_MIN, CONTROL_OUTPUT_MAX);

    return output;
}
