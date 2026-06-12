#ifndef PIR_H
#define PIR_H
#include "main.h"
// PIR SENSOR MODULE - GPIO: PC2 -> PIR OUTPUT
void PIR_Init(void);
uint8_t PIR_RawRead(void);
uint8_t PIR_HardMovementDetected(uint32_t required_time_ms);
#endif
