# DC FAN - fan_control.c Description

<img width="536" height="374" alt="image" src="https://github.com/user-attachments/assets/33e3eb97-cfb1-441b-a8f4-380add812709" />

<img width="1600" height="878" alt="WhatsApp Image 2026-06-20 at 10 13 36" src="https://github.com/user-attachments/assets/a0dcd133-130b-409e-a775-432e0d323d27" />



Fan Control Code Description

- #define FAN_TARGET_TEMP 22.0f // Desired temperature in °C: Threshold

- #define FAN_KP 25.0f // Proportional gain: It determines how strongly the fan reacts to temperature error.

<img width="152" height="34" alt="image" src="https://github.com/user-attachments/assets/a455905f-e712-48ea-bfdf-8868b98520d8" />
<img width="99" height="34" alt="image" src="https://github.com/user-attachments/assets/8c4a8de1-0f6a-4840-ba14-26cdb567317e" />
<img width="146" height="72" alt="image" src="https://github.com/user-attachments/assets/254334ee-d3c1-4ad3-a78c-47de4cb0e489" />

- #define FAN_DEADBAND 0.5f // Ignore small errors: If the temperature error is smaller than ±0.5°C, the controller ignores it and outputs zero.

- #define FAN_MIN_PWM 30 // Minimum PWM to start fan: DC fans do not start spinning at very low duty cycles (e.g., 5–20%).
They need a “kick” to overcome static friction.

- #define FAN_MAX_PWM 100 // Maximum PWM: This caps the fan speed at 100% duty cycle.

- #define DHT_READ_PERIOD_MS 2000 // DHT should not be read too fast: The DHT11 temperature–humidity sensors cannot be polled too frequently. They require ~2 seconds between reads.

- #define FAN_PWM_INVERTED 0 // FAN DRIVER TYPE: With a 2N2222 transistor when the PWM pin goes HIGH, current flows into the base, transistor turns ON, fan gets ground and fan spins.

Note:
0 = normal MOSFET / transistor driver, PWM HIGH = fan ON
1 = inverted driver, PWM LOW = fan ON

- static float current_temperature = 0.0f; This variable stores the latest measured temperature from the DHT sensor. static means persists across function calls. Initialized to 0.0 until the first sensor read.

- static float current_humidity = 0.0f; This variable stores the latest humidity reading from the DHT sensor.

- static uint8_t fan_pwm_percent = 0; This variable stores the current PWM duty cycle being applied to the fan, expressed in percent (0–100). uint8_t → 8‑bit integer (0–255), perfect for 0–100%.

- static uint32_t last_dht_read_time = 0; This stores the timestamp (in ms) of the last successful DHT read.

<img width="434" height="224" alt="image" src="https://github.com/user-attachments/assets/0cbc2c76-a5c5-4e78-8054-2f18b49f41f3" />

FanControl_Init(void):

- RCC->AHB1ENR |= (0x1 << 1); // Enable GPIOB clock: RCC AHB1 peripheral clock enable register (RCC_AHB1ENR) RCC = Reset and Clock Control. Select bit 1 from AHB1ENR that is GPIOB, then use OR bitwise compares to turn specifically this bit on (it turns ON the bit without affecting other bits), finally set the bit to 1.

<img width="331" height="125" alt="image" src="https://github.com/user-attachments/assets/16acf32a-ed42-4f66-89cb-8bc9147cabe9" />

- RCC->APB1ENR |= (0x1 << 2); // Enable TIM4 clock: RCC APB2 peripheral clock enable register (RCC_APB1ENR). Select bit 2 from APB1ENR that is TIM4, then use OR bitwise compares to turn specifically this bit on (it turns ON the bit without affecting other bits), finally set the bit to 1.

<img width="448" height="178" alt="image" src="https://github.com/user-attachments/assets/b7b651d2-9e2e-4791-a67e-7d96407770f3" />

- GPIOB->MODER &= ~(0x03 << 12); // Clear bits [13:12] - Apply Mask and force to 0. GPIOB->MODER |=  (0x02 << 12); // Set PA8 as Alternate Function (0010 - 0X02) - OR.    

<img width="461" height="280" alt="image" src="https://github.com/user-attachments/assets/6b39da4d-607a-4551-8585-f35383ef5682" />

- GPIOB->AFR[0] &= ~(0x0F << 24); / GPIOB->AFR[0] |=  (0x02 << 24); // Select AF2 for PB6. PB6 is in AFRL. AFRL bits for PB6 are [27:24]. AF2 = TIM4_CH1. It configures PB6 to be connected to TIM4 Channel 1 (PWM output). Session 7.4.9 and 7.4.10.
    
AFR = Alternate Function Register. It selects which peripheral is connected to the pin.

- <img width="259" height="85" alt="image" src="https://github.com/user-attachments/assets/d2d32fae-1f1d-4443-b8cc-32f77ebe9c2c" />

- <img width="324" height="212" alt="image" src="https://github.com/user-attachments/assets/614889a9-9e7d-4a91-a0e2-4b248d9ed609" />

Each pin needs 4 bits to choose its function. For PB6, we use bits 27:24.

GPIOB->AFR[2] &= ~(0x0F << 24): Clear the bits, 0x0F = 1111 (4 bits) then apply Mask and force to 0 the bits.

GPIOA->AFR[2] |= (0x01 << 24); Set the function, 0x01 = 0001, goes into bits [27:24] that is AF2.

<img width="223" height="122" alt="image" src="https://github.com/user-attachments/assets/13b0f37b-7ed7-4973-9cc6-1a93430cf58a" />

- GPIOB->OTYPER &= ~(0x1 << 6); // Push-pull output: Controls the output type of pins, each pin uses 1 bit only.

0x1 → one active bit and shift to position 6.
~ → apply bitwise NOT, this inverts all the bits.
&= → apply AND (the result is 1 only if both bits are 1). So, bit 8 is forced to 0 and all other bits remain unchanged.
Bit 6 = 0 → Push-Pull mode. To ensure the bit becomes 0, regardless of its previous value.
    
<img width="348" height="175" alt="image" src="https://github.com/user-attachments/assets/d3cb0876-3f5d-40af-8b49-951ddaed1958" />

- GPIOB->PUPDR &= ~(0x03 << 12); // No pull-up / pull-down: The instruction disables both pull-up and pull-down resistors on pin PB6, leaving it in a floating state (unless externally driven).
    
<img width="343" height="196" alt="image" src="https://github.com/user-attachments/assets/57571083-89cf-494c-918d-a2e471e3ab96" />

- GPIOB->OSPEEDR &= ~(0x03 << 12); / GPIOB->OSPEEDR |=  (0x02 << 12); // High speed output: GPIOB->OSPEEDR is the output speed register for port B on an STM32‑style MCU. Each pin uses 2 bits in this register.

GPIOB->OSPEEDR &= ~(0x03 << 12):
0x03 in binary is 0000 0011 → two bits set: 11. Shifts those  bits to position 12. 
~ → all bits 1 except those two → ... 1100 1111 1111 1111
&= with that inverted mask: This forces bits 13:12 to 0, leaving all other bits unchanged.

GPIOB->OSPEEDR |=  (0x02 << 12):
0x02 in binary is 0000 0010 → 10. Moves that 10 into bits 13:12:
|= with that mask: This sets bit 13:12 to 10 (Fast Speed)
    
<img width="449" height="302" alt="image" src="https://github.com/user-attachments/assets/8e4aabee-1ab9-48e6-bd10-3c73b18c26a5" />
 
TIM4 PWM Configuration

- TIM4->PSC = 83; // Timer clock assumed around 84 MHz. PSC = 83 gives 1 MHz timer frequency, 1 tick = 1 microsecond.

- <img width="161" height="35" alt="image" src="https://github.com/user-attachments/assets/3fd7fb61-3178-4744-b5d6-53cc7eee8a10" />

- <img width="255" height="37" alt="image" src="https://github.com/user-attachments/assets/39cd1c27-be3f-4b53-91fa-6303933292c7" />

- <img width="215" height="86" alt="image" src="https://github.com/user-attachments/assets/f1c2e506-cef7-4d8d-b788-a931ec961121" />

- <img width="330" height="121" alt="image" src="https://github.com/user-attachments/assets/c29ff3b0-86d6-4963-a449-f2a2c4e96551" />

- TIM4->ARR = 999; // So the timer counts from 0 → 999 = 1000 ticks. Since each tick is 1 µs: 1000 ticks = 1000 𝜇𝑠 = 1 ms.

- <img width="159" height="20" alt="image" src="https://github.com/user-attachments/assets/700a0a01-99bd-4223-bf3b-02ccc0ad65ab" />

- <img width="147" height="42" alt="image" src="https://github.com/user-attachments/assets/87189274-6a63-4298-a3bc-db80b4d3fec9" />

- <img width="332" height="116" alt="image" src="https://github.com/user-attachments/assets/25467f03-a578-438a-9916-18ec0bdb92a1" />

- TIM4->CCR1 = 0; // Start with fan OFF: CCR1 is the capture/compare register for channel 1 of TIM4. In PWM mode, CCR1 controls the duty cycle. ARR sets the period (you configured it to 999 → 1 kHz PWM). CCR1 sets the pulse width inside that period.
    
- TIM4->CCMR1 &= ~(0x7 << 4); // PWM mode 1 on channel 1: CCMR1: It controls how channel 1 and channel 2 of the timer behave. For channel 1, the important bits are: Bits [6:4] → OC1M (Output Compare Mode)

<img width="348" height="261" alt="image" src="https://github.com/user-attachments/assets/5c8f1fed-16ef-425e-b13b-ebdeb288185a" />

(0x7 << 4) → 0b1110000
~(0x7 << 4) → invert → clears bits 6, 5, 4
&= applies the mask
Result: Bits 6, 5, 4 are set to 0, all other bits stay unchanged.

- TIM4->CCMR1 |=  (0x6 << 4); Set PWM Mode 1. Output is: HIGH when counter < CCR1, LOW when counter ≥ CCR1.

0x6 = 0b110
Shift left by 4
|= sets those bits
- <img width="211" height="59" alt="image" src="https://github.com/user-attachments/assets/6068bed2-8067-411a-b6d9-3f08842d70f4" />
- <img width="295" height="186" alt="image" src="https://github.com/user-attachments/assets/67077386-2112-43b6-9641-59fcf9dcbb00" />

- TIM4->CCMR1 |=  (0x1 << 3); // Enable preload: It means the value you write to CCR1 is not applied immediately. Instead, it is stored in a shadow register and it only takes effect at the next timer update event (when counter resets). Without preload the PWM signal might glitch if you update CCR1 in the middle of a cycle.

<img width="281" height="58" alt="image" src="https://github.com/user-attachments/assets/e48658ce-7039-49d8-bdfd-9aee3d527c34" />

- TIM4->CCER |= (0x1 << 0); // Enable channel 1 output: Enable channel 1 output: Controls whether each channel output is actually sent to the pin. Without this, the timer runs, PWM is generated internally but nothing appears on the pin.

<img width="286" height="66" alt="image" src="https://github.com/user-attachments/assets/9c65b768-21c1-4aec-aeec-b8addc1d6c39" />

- TIM4->CR1 |= (0x1 << 7); // Enable auto-reload preload: Enable auto-reload preload: CR1 = Control Register 1, it controls the main behavior of the timer. New ARR value is stored in a shadow register. It only takes effect at the next update event (counter reset).

<img width="334" height="80" alt="image" src="https://github.com/user-attachments/assets/ca3a48dd-a4a7-4620-8aea-d14459b1ae9a" />
<img width="129*0.5" height="27*0.5" alt="image" src="https://github.com/user-attachments/assets/d32c156e-7268-487b-b199-79ea1fab989a" />


- TIM4->EGR |= (0x1 << 0); // Generate update event: Event Generation Register. It forces the timer to immediately reload the PSC and ARR values you just configured, instead of waiting for the next timer overflow.

<img width="446" height="109" alt="image" src="https://github.com/user-attachments/assets/10ebaa7b-e274-42aa-98b8-e2563d9a0313" />
<img width="375" height="79" alt="image" src="https://github.com/user-attachments/assets/11e143e0-6697-4d2d-9460-68aff6c9ad37" />

- TIM4->CR1 |= (0x1 << 0); // Enable timer
<img width="280*0.3" height="49*0.3" alt="image" src="https://github.com/user-attachments/assets/a242fcf8-4ac9-4feb-a04a-bc7cd675625d" />

---- Functions ---- 

- static void Fan_SetPWM(uint8_t duty_percent): set the fan PWM duty cycle in percent (0–100) and translate that into the correct CCR1 value for TIM4 channel 1.

This part ensures you never exceed 100%

- if (duty_percent > 100)
    {
        duty_percent = 100;
    }

This part store the current duty for later use.

- fan_pwm_percent = duty_percent;

This part is compiled when #define FAN_PWM_INVERTED 0, which is our case the 2N2222.

- TIM4->CCR1 = (duty_percent * (TIM4->ARR + 1)) / 100;

Step by step: TIM4-> ARR = 999 → ARR + 1 = 1000 counts.

duty_percent * (ARR + 1). Example: 25% → 25 * 1000 = 25000. Divide by 100 to get the compare value: 25000 / 100 = 250.

This gives:
0% → CCR1 = 0
50% → CCR1 = 500
100% → CCR1 = 1000 (or 999 depending on rounding)

- void FanControl_Update(void): 

This part returns the system time in milliseconds, now is the current timestamp used to decide if it’s time to read the DHT sensor again.

- uint32_t now = HAL_GetTick();

This part computes how long it’s been since the last DHT read: now - last_dht_read_time. If this elapsed time is less than DHT_READ_PERIOD_MS (2000 ms), the function exits immediately. This means: “Too soon to read the sensor again, do nothing this cycle.” If enough time has passed, we continue.

- if ((now - last_dht_read_time) < DHT_READ_PERIOD_MS)
    {
        return;
    }

This part records that we’re about to perform a new DHT read at time now. Next call will measure time from this moment.

- last_dht_read_time = now;

The last part function is: DHT_Read(...) attempts to read the sensor. On success (returns non‑zero/true): It updates current_temperature and current_humidity. Then we compute the control action. On failure: For safety, the fan is turned OFF: Fan_SetPWM(0);.

1. Compute temperature error. error = measured temperature − target temperature. If current_temperature > target → error > 0 → we need cooling. If current_temperature ≤ target → error ≤ 0 → no need to cool, pwm_value will hold the fan speed in percent (0–100). 

- float error = current_temperature - FAN_TARGET_TEMP;
- float pwm_value = 0.0f;

2. Apply deadband. If the error is less than or equal to the deadband (e.g. 0.5°C), the fan is kept OFF. This avoids fan activity for tiny temperature deviations and sensor noise.

3. Proportional control when error is significant. When error > FAN_DEADBAND, we compute a proportional control. Larger temperature difference → higher PWM → faster fan.

- pwm_value = FAN_KP * error;

4. Enforce minimum PWM. If the computed PWM is too small (below the minimum needed to reliably start the fan), it is raised to FAN_MIN_PWM (e.g. 30%). This avoids the fan stalling or buzzing at very low duty cycles.

- if (pwm_value < FAN_MIN_PWM)
{
    pwm_value = FAN_MIN_PWM;
}

5. Enforce maximum PWM. Caps the PWM at FAN_MAX_PWM (e.g. 100%). Protects the fan and ensures the duty cycle stays within valid bounds.

- if (pwm_value > FAN_MAX_PWM)
{
    pwm_value = FAN_MAX_PWM;
}

6. Apply the PWM to the hardware. Converts the float pwm_value to an 8‑bit integer (0–100). Calls Fan_SetPWM, which: stores fan_pwm_percent, converts the percentage into a CCR1 value using ARR, updates the timer’s PWM output.

- Fan_SetPWM((uint8_t)pwm_value);

7. Sensor failure branch. If the DHT read fails, the system does not trust the temperature. As a safety measure, it turns the fan OFF instead of running it blindly.

- else
{
    // If DHT reading fails, turn fan OFF for safety.
    Fan_SetPWM(0);
}
