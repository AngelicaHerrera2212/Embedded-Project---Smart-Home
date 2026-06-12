#include "leds.h"
/* LED CONTROL MODULE
 * CONTROLS: 5 room LEDs
 * FUNCTIONS:
 * - Initialize LEDs
 * - Turn ON all LEDs
 * - Turn OFF all LEDs
 */
void LEDs_Init(void)
{
    // ENABLE CLOCKS
    RCC->AHB1ENR |= (0x1 << 1); // GPIOB
    RCC->AHB1ENR |= (0x1 << 2); // GPIOC
    /*
     * PC3 -> LED1
     * PC4 -> LED2
     * PC5 -> LED3
     */
    GPIOC->MODER &= ~(0x03 << 6);
    GPIOC->MODER |=  (0x01 << 6);

    GPIOC->MODER &= ~(0x03 << 8);
    GPIOC->MODER |=  (0x01 << 8);

    GPIOC->MODER &= ~(0x03 << 10);
    GPIOC->MODER |=  (0x01 << 10);
    /* PB0 -> LED4
     * PB1 -> LED5 */
    GPIOB->MODER &= ~(0x03 << 0);
    GPIOB->MODER |=  (0x01 << 0);

    GPIOB->MODER &= ~(0x03 << 2);
    GPIOB->MODER |=  (0x01 << 2);
    LEDs_Off();
}

 // TURN ON ALL LEDs
void LEDs_On(void)
{
    GPIOC->ODR |= (0x1 << 3);
    GPIOC->ODR |= (0x1 << 4);
    GPIOC->ODR |= (0x1 << 5);
    GPIOB->ODR |= (0x1 << 0);
    GPIOB->ODR |= (0x1 << 1);
}

 // TURN OFF ALL LEDs
void LEDs_Off(void)
{
    GPIOC->ODR &= ~(0x1 << 3);
    GPIOC->ODR &= ~(0x1 << 4);
    GPIOC->ODR &= ~(0x1 << 5);
    GPIOB->ODR &= ~(0x1 << 0);
    GPIOB->ODR &= ~(0x1 << 1);
}
