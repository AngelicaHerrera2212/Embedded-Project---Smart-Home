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
 *
 * * Software logic:
 * - We do NOT trust one single PIR HIGH signal.
 * - We require several consecutive HIGH readings.
 * - This helps ignore small noise or weak movement.
 */

#define PIR_PORT GPIOC
#define PIR_PIN_NUMBER 2

#define PIR_REQUIRED_HIGH_READINGS  5
#define PIR_SAMPLE_DELAY_MS         20
#define PIR_COOLDOWN_MS             800

void PIR_Init(void)
{
    // ENABLE GPIOC CLOCK
    RCC->AHB1ENR |= (0x1 << 2);
    /* CONFIGURE PC2 AS INPUT
     * PC2 -> bits [5:4] */
    GPIOC->MODER &= ~(0x03 << 4);
    // ENABLE PULL-DOWN
    /* PUPDR PC2 uses bits [5:4]
    * 00 = no pull
    * 01 = pull-up
    * 10 = pull-down*/
    GPIOC->PUPDR &= ~(0x03 << 4);
    GPIOC->PUPDR |= (0x02 << 4);
}
 /* RAW PIR READ
 * RETURNS:
 * 1 -> movement detected
 * 0 -> no movement
 */
uint8_t PIR_Detected(void)
{
    if(GPIOC->IDR & (0x1 << PIR_PIN_NUMBER))
    {
        return 1;
    }
    return 0;
}

/* HARD MOVEMENT DETECTION
 *
 * This function filters the PIR signal. It only returns 1 if the PIR stays HIGH for
 * multiple samples.
 * Returns:
 * 1 -> valid strong movement
 * 0 -> ignored / no movement
 */
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
