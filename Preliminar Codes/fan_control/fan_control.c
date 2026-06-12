#include "fan_control.h"
#include "dht.h"
/* FAN + DHT CONTROL
 * Control approach: error = current_temperature - target_temperature
 * If error <= 0:
 *      fan OFF
 * If error > 0:
 *      fan speed increases depending on the error
 * This is a proportional control approach, similar to a simple PID
 * but using only the P term. The function FanControl_Update() should
 * be called repeatedly inside while(1).
 * HAL_GetTick() is used only to avoid reading the DHT too fast. */

// USER CONFIGURATION

#define FAN_TARGET_TEMP       22.0f   // Desired temperature in °C
#define FAN_KP                25.0f   // Proportional gain
#define FAN_DEADBAND          0.5f    // Ignore small errors
#define FAN_MIN_PWM           30      // Minimum PWM to start fan
#define FAN_MAX_PWM           100     // Maximum PWM

#define DHT_READ_PERIOD_MS    2000    // DHT should not be read too fast

/* FAN DRIVER TYPE:
 * 0 = normal MOSFET / transistor driver, PWM HIGH = fan ON
 * 1 = inverted driver, PWM LOW = fan ON
 *
 * If you use a normal NPN/MOSFET low-side driver, keep this as 0. */

#define FAN_PWM_INVERTED      0

static float current_temperature = 0.0f;
static float current_humidity = 0.0f;
static uint8_t fan_pwm_percent = 0;

static uint32_t last_dht_read_time = 0;


/* FAN PWM INITIALIZATION
 * PB6 is configured as Alternate Function 2:
 * PB6 -> TIM4_CH1
 *
 * TIM4 generates PWM signal for fan speed control. */
void FanControl_Init(void)
{
    // Enable GPIOB clock
    RCC->AHB1ENR |= (0x1 << 1);

    // Enable TIM4 clock
    RCC->APB1ENR |= (0x1 << 2);

    /* Configure PB6 as Alternate Function
     * PB6 MODER bits [13:12]
     * 00 = input
     * 01 = output
     * 10 = alternate function
     * 11 = analog */
    GPIOB->MODER &= ~(0x03 << 12);
    GPIOB->MODER |=  (0x02 << 12);

    /* Select AF2 for PB6
     * PB6 is in AFRL.
     * AFRL bits for PB6 are [27:24]
     * AF2 = TIM4_CH1 */
    GPIOB->AFR[0] &= ~(0x0F << 24);
    GPIOB->AFR[0] |=  (0x02 << 24);

    // Push-pull output
    GPIOB->OTYPER &= ~(0x1 << 6);

    // No pull-up / pull-down
    GPIOB->PUPDR &= ~(0x03 << 12);

    // High speed output
    GPIOB->OSPEEDR &= ~(0x03 << 12);
    GPIOB->OSPEEDR |=  (0x02 << 12);

    /* Timer configuration:
     * System clock is approximately 84 MHz.
     * TIM4 clock is approximately 84 MHz.
     *
     * PWM frequency = Timer clock / (PSC + 1) / (ARR + 1)
     *
     * PSC = 83  -> timer counts at 1 MHz
     * ARR = 999 -> PWM frequency = 1 kHz */
    TIM4->PSC = 83;
    TIM4->ARR = 999;

    // Start with fan OFF
    TIM4->CCR1 = 0;

    /* PWM mode 1 on channel 1
     * OC1M = 110 -> PWM mode 1
     * OC1PE = 1  -> preload enable */
    TIM4->CCMR1 &= ~(0x7 << 4);
    TIM4->CCMR1 |=  (0x6 << 4);
    TIM4->CCMR1 |=  (0x1 << 3);

    // Enable channel 1 output
    TIM4->CCER |= (0x1 << 0);

    // Enable auto-reload preload
    TIM4->CR1 |= (0x1 << 7);

    // Generate update event
    TIM4->EGR |= (0x1 << 0);

    // Enable timer
    TIM4->CR1 |= (0x1 << 0);
}

/* SET FAN PWM - duty_percent:
 * 0   -> fan OFF
 * 100 -> fan full speed */
static void Fan_SetPWM(uint8_t duty_percent)
{
    if (duty_percent > 100)
    {
        duty_percent = 100;
    }

    fan_pwm_percent = duty_percent;

#if FAN_PWM_INVERTED == 0
    TIM4->CCR1 = (duty_percent * (TIM4->ARR + 1)) / 100;
#else
    TIM4->CCR1 = ((100 - duty_percent) * (TIM4->ARR + 1)) / 100;
#endif
}

/* FAN CONTROL UPDATE
 * This function:
 * 1. Reads DHT temperature/humidity every 2 seconds.
 * 2. Calculates temperature error.
 * 3. Converts error into PWM fan speed. */
void FanControl_Update(void)
{
    uint32_t now = HAL_GetTick();

    if ((now - last_dht_read_time) < DHT_READ_PERIOD_MS)
    {
        return;
    }

    last_dht_read_time = now;

    if (DHT_Read(&current_temperature, &current_humidity))
    {
        float error = current_temperature - FAN_TARGET_TEMP;
        float pwm_value = 0.0f;

        if (error <= FAN_DEADBAND)
        {
            pwm_value = 0.0f;
        }
        else
        {
            pwm_value = FAN_KP * error;

            if (pwm_value < FAN_MIN_PWM)
            {
                pwm_value = FAN_MIN_PWM;
            }

            if (pwm_value > FAN_MAX_PWM)
            {
                pwm_value = FAN_MAX_PWM;
            }
        }

        Fan_SetPWM((uint8_t)pwm_value);
    }
    else
    {
        // If DHT reading fails, turn fan OFF for safety.
        Fan_SetPWM(0);
    }
}

float FanControl_GetTemperature(void)
{
    return current_temperature;
}

float FanControl_GetHumidity(void)
{
    return current_humidity;
}

uint8_t FanControl_GetPWM(void)
{
    return fan_pwm_percent;
}
