#include "dht.h"
/* DHT11 / DHT22 MODULE
* GPIO ASSIGNMENT:
* PC1 -> DHT DATA PIN
*
* CONNECTIONS:
* VCC  -> 5V
* GND  -> GND
* DATA -> PC1
*
* IMPORTANT:
* 10k pull-up resistor between DATA and VCC
*
* FUNCTIONS:
* - Configure DHT pin
* - Read temperature
* - Read humidity
*/
#define DHT_PORT GPIOC
#define DHT_PIN  GPIO_PIN_1

/*MICROSECOND DELAY*/
static void delay_us(uint32_t us)
{
   uint32_t start = DWT->CYCCNT;
   uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);
   while ((DWT->CYCCNT - start) < ticks);
}
/*INITIALIZE DHT MODULE*/
void DHT_Init(void)
{
   /*ENABLE GPIOC CLOCK*/
   RCC->AHB1ENR |= (0x1 << 2);
   /* ENABLE DWT COUNTER
    * Used for microsecond timing */
   CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
   DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
/*
* CONFIGURE PC1 AS OUTPUT
* Used during DHT START SIGNAL*/
static void DHT_SetOutput(void)
{
   /*PC1 -> bits [3:2]*/
   GPIOC->MODER &= ~(0x03 << 2);
   GPIOC->MODER |=  (0x01 << 2);
   /*PUSH-PULL*/
   GPIOC->OTYPER &= ~(0x1 << 1);
   /*NO PULL-UP / PULL-DOWN*/
   GPIOC->PUPDR &= ~(0x03 << 2);
}
/*CONFIGURE PC1 AS INPUT
* Used during DATA RECEPTION
*/
static void DHT_SetInput(void)
{
   /*INPUT MODE*/
   GPIOC->MODER &= ~(0x03 << 2);
   /*ENABLE PULL-UP*/
   GPIOC->PUPDR &= ~(0x03 << 2);
   GPIOC->PUPDR |=  (0x01 << 2);
}
/*READ DHT SENSOR
*
* RETURNS:
* 1 -> SUCCESS
* 0 -> ERROR
*
*/
uint8_t DHT_Read(float *temperature, float *humidity)
{
   uint8_t data[5] = {0};
   uint32_t timeout;
   /*SEND START SIGNAL*/
   DHT_SetOutput();
   GPIOC->ODR &= ~(0x1 << 1);
   HAL_Delay(18);
   GPIOC->ODR |= (0x1 << 1);
   delay_us(30);
   /*SWITCH TO INPUT MODE*/
   DHT_SetInput();
   /*WAIT FOR DHT RESPONSE*/
   timeout = 0;
   while(GPIOC->IDR & (0x1 << 1))
   {
       if(++timeout > 10000)
           return 0;
   }
   timeout = 0;
   while(!(GPIOC->IDR & (0x1 << 1)))
   {
       if(++timeout > 10000)
           return 0;
   }
   timeout = 0;
   while(GPIOC->IDR & (0x1 << 1))
   {
       if(++timeout > 10000)
           return 0;
   }
   /*READ 40 BITS*/
   for(int i = 0; i < 40; i++)
   {
       timeout = 0;
       while(!(GPIOC->IDR & (0x1 << 1)))
       {
           if(++timeout > 10000)
               return 0;
       }
       delay_us(40);
       if(GPIOC->IDR & (0x1 << 1))
       {
           data[i / 8] |= (1 << (7 - (i % 8)));
       }
       timeout = 0;
       while(GPIOC->IDR & (0x1 << 1))
       {
           if(++timeout > 10000)
               return 0;
       }
   }
   /*CHECKSUM VALIDATION*/
   if((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
   {
       return 0;
   }
   /*STORE VALUES*/
   *humidity = data[0];
   *temperature = data[2];
   return 1;
}
