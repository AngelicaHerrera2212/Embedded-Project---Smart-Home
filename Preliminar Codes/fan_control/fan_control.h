#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H
#include "main.h"
/* FAN + DHT CONTROL MODULE
 * Fan PWM pin: PB6 -> TIM4_CH1
 * DHT pin: PC1 -> handled by dht.c */
void FanControl_Init(void);
void FanControl_Update(void);
float FanControl_GetTemperature(void);
float FanControl_GetHumidity(void);
uint8_t FanControl_GetPWM(void);
#endif
