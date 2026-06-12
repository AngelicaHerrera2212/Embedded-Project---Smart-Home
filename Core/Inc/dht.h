#ifndef DHT_H
#define DHT_H
// DHT11 / DHT22 MODULE HEADER
#include "main.h"
void DHT_Init(void);
uint8_t DHT_Read(float *temperature, float *humidity);
#endif
