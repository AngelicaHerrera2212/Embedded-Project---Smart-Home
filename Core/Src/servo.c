#include "servo.h"
/* SERVO DOOR CONTROL MODULE
 * GPIO: PA8 -> Servo signal pin
 * Timer: TIM1_CH1
 * Servo connection: Brown/Black -> GND
 * Red -> 5V from MB-102
 * Orange/Yellow -> PA8 */

// Adjust these values by trial and error
#define SERVO_CLOSED_ANGLE   180
#define SERVO_OPEN_ANGLE     0

/* Servo pulse range:
 * Usually:
 * 0°   -> 1 ms
 * 90°  -> 1.5 ms
 * 180° -> 2 ms */

#define SERVO_MIN_PULSE_US   1000
#define SERVO_MAX_PULSE_US   2000

void Servo_Init(void)
{
    // Enable GPIOA clock
    RCC->AHB1ENR |= (0x1 << 0);

    // Enable TIM1 clock
    RCC->APB2ENR |= (0x1 << 0);

    /* Configure PA8 as Alternate Function
     * PA8 MODER bits [17:16]
     * 10 = alternate function */
    GPIOA->MODER &= ~(0x03 << 16); // Clear bits [17:16] - Apply Mask and force to 0
    GPIOA->MODER |=  (0x02 << 16); // Set PA8 as OUTPUT - OR

    // Select AF1 for PA8. PA8 -> TIM1_CH1
    GPIOA->AFR[1] &= ~(0x0F << 0); // Clear bits [3:0] - Apply Mask and force to 0
    GPIOA->AFR[1] |=  (0x01 << 0); //

    GPIOA->OTYPER &= ~(0x1 << 8); // Push-pull,
    GPIOA->PUPDR  &= ~(0x03 << 16); // no pull-up/down

    /* TIM1 PWM configuration
     * Timer clock assumed around 84 MHz.
     * PSC = 83 gives 1 MHz timer frequency.
     * 1 tick = 1 microsecond.
     *
     * Servo needs 50 Hz:
     * Period = 20 ms = 20000 us. */
    TIM1->PSC = 84 - 1;
    TIM1->ARR = 20000 - 1;

    // PWM mode 1 on channel 1
    TIM1->CCMR1 &= ~(0x7 << 4);
    TIM1->CCMR1 |=  (0x6 << 4);

    // Enable preload
    TIM1->CCMR1 |= (0x1 << 3);

    // Enable channel 1 output
    TIM1->CCER |= (0x1 << 0);

    // TIM1 is advanced-control timer, so Main Output Enable is required.
    TIM1->BDTR |= (0x1 << 15);

    // Enable auto-reload preload
    TIM1->CR1 |= (0x1 << 7);

    // Start timer
    TIM1->CR1 |= (0x1 << 0);

    // Initial position
    Servo_CloseDoor();
}

void Servo_SetAngle(uint8_t angle)
{
    if (angle > 180)
    {
        angle = 180;
    }

    /* Convert angle to pulse width.
     * pulse_us = 500 + angle * (2000 / 180)*/
    uint32_t pulse_us =
        SERVO_MIN_PULSE_US +
        ((uint32_t)angle * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)) / 180;

    // CCR1 controls pulse width in microseconds.
    TIM1->CCR1 = pulse_us;
}

void Servo_OpenDoor(void)
{
    Servo_SetAngle(SERVO_OPEN_ANGLE);
}

void Servo_CloseDoor(void)
{
    Servo_SetAngle(SERVO_CLOSED_ANGLE);
}
