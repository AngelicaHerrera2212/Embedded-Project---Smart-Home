#include "pir.h"
/* PIR SENSOR MODULE
 * GPIO ASSIGNMENT: PC2 -> PIR OUTPUT SIGNAL
 * FUNCTIONS:
 * - Initialize PIR pin
 * - Detect movement
 *
 * Hardware notes:
 * - Set PIR sensitivity potentiometer to minimum.
 * - Set delay potentiometer to minimum.
 */

#define PIR_PIN_NUMBER 2

/*#define PIR_REQUIRED_HIGH_READINGS  10
#define PIR_SAMPLE_DELAY_MS         20
#define PIR_COOLDOWN_MS             800*/

void PIR_Init(void)
{
    RCC->AHB1ENR |= (0x1 << 2); // ENABLE GPIOC CLOCK
    GPIOC->MODER &= ~(0x03 << 4); // CONFIGURE PC2 AS INPUT, PC2 -> bits [5:4]
    // ENABLE PULL-DOWN
    /* PUPDR PC2 uses bits [5:4]
    * 00 = no pull
    * 01 = pull-up
    * 10 = pull-down*/
    GPIOC->PUPDR &= ~(0x03 << 4);
    //GPIOC->PUPDR |= (0x02 << 4);
}
 /* RAW PIR READ
 * RETURNS:
 * 1 -> movement detected
 * 0 -> no movement */
uint8_t PIR_RawRead(void)
{
    if(GPIOC->IDR & (0x1 << PIR_PIN_NUMBER))
    {
        return 1;
    }
    return 0;
}

/* HARD MOVEMENT DETECTION
 *
 * This function filters the PIR signal. It only returns 1 if the PIR motion remain
 * HIGH for some specific time. For example 0.5 seconds (500 ms)
 * 1. Detect first HIGH signal
 * 2. Start timer
 * 3. Continue checking signal
 * 4. If signal drops LOW -> reject
 * 5. If HIGH long enough -> VALID motion */

uint8_t PIR_HardMovementDetected(uint32_t required_time_ms)
{
    uint32_t elapsed_time = 0;


     //SAMPLE PERIOD. Every 10 ms we check the PIR signal
     const uint32_t sample_period_ms = 10;
    //WAIT FOR INITIAL MOTION
    if(PIR_RawRead())
    {
        //CONTINUE WHILE SIGNAL REMAINS HIGH
        while(PIR_RawRead())
        {
            HAL_Delay(sample_period_ms);
            elapsed_time += sample_period_ms;
            // VALID MOVEMENT
            if(elapsed_time >= required_time_ms)
            {
                return 1;
            }
        }
    }

    // SIGNAL DID NOT LAST LONG ENOUGH
    return 0;
}

/*
uint8_t PIR_HardMovementDetected(void)
{
    uint8_t high_count = 0;
    for (int i = 0; i < PIR_REQUIRED_HIGH_READINGS; i++)
    {
        if (PIR_RawRead())
        {
            high_count++;
        }
        HAL_Delay(PIR_SAMPLE_DELAY_MS);
    }
    if (high_count >= PIR_REQUIRED_HIGH_READINGS)
    {
        HAL_Delay(PIR_COOLDOWN_MS);
        return 1;
    }
    return 0;
}
*/

