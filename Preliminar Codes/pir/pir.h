#ifndef PIR_H
#define PIR_H
#include "main.h"
// PIR SENSOR MODULE - GPIO: PC2 -> PIR OUTPUT
void PIR_Init(void);
uint8_t PIR_Detected(void);
#endif
