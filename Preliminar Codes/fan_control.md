# DC FAN - fan_control.c Description

<img width="536" height="374" alt="image" src="https://github.com/user-attachments/assets/33e3eb97-cfb1-441b-a8f4-380add812709" />

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

- GPIOB->MODER &= ~(0x03 << 12); // Clear bits [13:12] - Apply Mask and force to 0. GPIOB->MODER |=  (0x02 << 12); // Set PA8 as Alternate Function - OR.    

<img width="461" height="280" alt="image" src="https://github.com/user-attachments/assets/6b39da4d-607a-4551-8585-f35383ef5682" />


