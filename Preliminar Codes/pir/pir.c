#include "pir.h"
/* PIR SENSOR MODULE
 * GPIO ASSIGNMENT: PC2 -> PIR OUTPUT SIGNAL
 * FUNCTIONS:
 * - Initialize PIR pin
 * - Detect movement
 */
void PIR_Init(void)
{
    // ENABLE GPIOC CLOCK
    RCC->AHB1ENR |= (0x1 << 2);
    /* CONFIGURE PC2 AS INPUT
     * PC2 -> bits [5:4] */
    GPIOC->MODER &= ~(0x03 << 4);
    // ENABLE PULL-DOWN
    GPIOC->PUPDR &= ~(0x03 << 4);
    GPIOC->PUPDR |= (0x02 << 4);
}
 /* RETURNS:
 * 1 -> movement detected
 * 0 -> no movement
 */
uint8_t PIR_Detected(void)
{
    if(GPIOC->IDR & (0x1 << 2))
    {
        return 1;
    }
    return 0;
}
