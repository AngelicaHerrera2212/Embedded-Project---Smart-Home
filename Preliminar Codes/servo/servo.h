#ifndef SERVO_H
#define SERVO_H
#include "main.h"
/* SERVO MODULE
 * Servo signal pin: PA8 -> TIM1_CH1
 * Used for door opening/closing.
 */
void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);
void Servo_OpenDoor(void);
void Servo_CloseDoor(void);
#endif
