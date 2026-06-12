#ifndef LEDS_H
#define LEDS_H
#include "main.h"
/* LED MODULE
 * LED GPIO ASSIGNMENTS:
 * PC3 -> LED1
 * PC4 -> LED2
 * PC5 -> LED3
 * PB0 -> LED4
 * PB1 -> LED5
 */
void LEDs_Init(void);
void LEDs_On(void);
void LEDs_Off(void);
#endif
