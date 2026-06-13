# SERVO SG90 - servo.c Description

To connect the servo motor, we used the diagrams below.

- <img width="289" height="208" alt="image" src="https://github.com/user-attachments/assets/a53ea6ee-5289-4fc6-aec2-9331f157c9ab" />
- <img width="289" height="394" alt="image" src="https://github.com/user-attachments/assets/b4ebc1ce-713e-4e2f-a820-40bdbf645e27" />

Servo for Door Control Code Description.

- #define SERVO_CLOSED_ANGLE   0 / #define SERVO_OPEN_ANGLE     90

These are just labels (names) for angles, in as this angles will be used for the door control we can modify the variables values depending on the physical performance.

- Comments section: Servos don’t understand “angle” directly. They understand PWM (time).

Note from class: for many servomotors a PWM signal with a period of 20ms (50Hz) is used and the variation of the duty cycle determines the variation of the motor rod angle, from 0 to 180 degrees (usually).
/* Servo pulse range usually:
 1. 0°   -> 1 ms (1000 µs)
 2. 90°  -> 1.5 ms (1500 µs)
 3. 180° -> 2 ms (2000 µs) */

<img width="324" height="203" alt="image" src="https://github.com/user-attachments/assets/f3703cef-2d22-414b-a503-14b3d640cc9d" />
<img width="392" height="203" alt="image" src="https://github.com/user-attachments/assets/4fb19ea7-6065-4c2f-b88f-1a961c8c9039" />
<img width="389" height="171" alt="image" src="https://github.com/user-attachments/assets/d9a8d4c0-da83-40e5-8d4c-f3dd1b553d33" />

- #define SERVO_MIN_PULSE_US   1000 / #define SERVO_MAX_PULSE_US   2000

These are the boundaries of the servo movement. 1000 µs = minimum pulse → 0°, 2000 µs = maximum pulse → 180°. These are un microseconds due to the part of the code that configures the timer to 20ms, 50Hz. As servo except a pulse every 20 ms (50 Hz)
The HIGH time determines the position. As the required precision is small: 1 ms = 1000 µs, and knowing that servo changes position with differences of ~10–20 µs.

<img width="245" height="262" alt="image" src="https://github.com/user-attachments/assets/39408d38-8b23-437f-8b1d-65208a06e445" />

The STM32 Timer is like a counter:

<img width="362" height="62" alt="image" src="https://github.com/user-attachments/assets/2e021655-f09d-47f1-a73a-973d6722dc73" />

1. PSC — Prescaler: IM1->PSC = 84 - 1. Formula: Timer frequency = CPU clock / (PSC + 1).

<img width="366" height="229" alt="image" src="https://github.com/user-attachments/assets/e31db7d9-40a3-489e-b0d2-797a9552a722" />

2. ARR — Auto-Reload Register (period): TIM1->ARR = 19999. It defines when the timer resets. Formula: Period = (ARR + 1) × tick_time.

<img width="369" height="170" alt="image" src="https://github.com/user-attachments/assets/cce37a77-8562-45a8-91eb-c4a978b0e1a2" />

And this five as result: PWM frequency = 1 / 20 ms = 50 Hz.

3. CCR1 — Compare Register (pulse width): TIM1->CCR1 = pulse_us. This controls how long the signal stays HIGH, and that is what we change in our functions depending on the desired angle.

<img width="369" height="282" alt="image" src="https://github.com/user-attachments/assets/270ddd80-2838-478f-9e6c-adb827a5db2a" />

<img width="368" height="170" alt="image" src="https://github.com/user-attachments/assets/2d3d28d8-6ad0-4507-998b-786aaec987c6" />


Value in register / Real meaning

CCR1 = 1000 / 1000 µs = 1 ms

CCR1 = 1500 / 1500 µs = 1.5 ms

CCR1 = 2000 / 2000 µs = 2 ms

- void Servo_Init(void): contents the configuration of the registers for the servo module.

1. RCC->AHB1ENR |= (0x1 << 0); // Enable GPIOA clock. Session 6.3.10 - RCC AHB1 peripheral clock enable register (RCC_AHB1ENR) RCC = Reset and Clock Control. Select bit 0 from AHB1ENR that is GPIOA, then use OR bitwise compares to turn specifically this bit on (it turns ON the bit without affecting other bits), finally set the bit to 1.

 <img width="449" height="371" alt="image" src="https://github.com/user-attachments/assets/ef22c6ea-47a1-491d-87e5-54a39adc5ff5" />
 
2. RCC->APB2ENR |= (0x1 << 0); // Enable TIM1 clock. Session 6.3.14 - RCC APB2 peripheral clock enable register (RCC_APB2ENR). Select bit 0 from APB2ENR that is TIM1, then use OR bitwise compares to turn specifically this bit on (it turns ON the bit without affecting other bits), finally set the bit to 1.

 <img width="449" height="371" alt="image" src="https://github.com/user-attachments/assets/3dc47178-fcd8-46bc-a4dd-4ffc616388dd" />
Datasheet reference:
<img width="383" height="368" alt="image" src="https://github.com/user-attachments/assets/5e0b72ed-2a1c-419b-9691-c0de6437e629" />
<img width="622" height="394" alt="image" src="https://github.com/user-attachments/assets/bba4748c-40ee-40bd-a95b-1eafa6fc4a3a" />

3. GPIOA->MODER &= ~(0x03 << 16); // Clear bits [17:16] - Apply Mask and force to 0.
GPIOA->MODER |=  (0x02 << 16); // Set PA8 as OUTPUT - OR.

<img width="440" height="277" alt="image" src="https://github.com/user-attachments/assets/72e0bbfd-e0c7-4181-8294-38713c6e017b" />

4. GPIOA->AFR[1] &= ~(0x0F << 0); / GPIOA->AFR[1] |=  (0x01 << 0); // Select AF1 for PA8. PA8 -> TIM1_CH1. It configures PA8 to be connected to TIM1 Channel 1 (PWM output). Session 7.4.9 and 7.4.10.

AFR = Alternate Function Register. It selects which peripheral is connected to the pin. STM32 splits AFR into two registers: AFR[0] is AFRL (low) and AFR[1] is AFRH (high).

- <img width="310" height="91" alt="image" src="https://github.com/user-attachments/assets/4973417a-f21f-40d9-98a2-86c8a24f8424" />

- <img width="440" height="287" alt="image" src="https://github.com/user-attachments/assets/09c71e4c-f732-4ba9-a2ff-7b6fac661efa" />

- <img width="440" height="317" alt="image" src="https://github.com/user-attachments/assets/b40ce80a-308b-497d-abcd-b546abab2bc7" />

Each pin needs 4 bits to choose its function. For PA8, we use bits 3:0. 

GPIOA->AFR[1] &= ~(0x0F << 0): Clear the bits, 0x0F = 1111 (4 bits) then apply Mask and force to 0 the bits.

GPIOA->AFR[1] |=  (0x01 << 0); Set the function, 0x01 = 0001, goes into bits [3:0] that is AF1.

<img width="305" height="166" alt="image" src="https://github.com/user-attachments/assets/045d3f7a-a9fb-4f94-9cd0-93c12712968c" />

5. GPIOA->OTYPER &= ~(0x1 << 8); // Push-pull: Controls the output type of pins, each pin uses 1 bit only

- 0x1 → one active bit and shift to position 8.
- ~ → apply bitwise NOT, this inverts all the bits.
- &= → apply AND (the result is 1 only if both bits are 1). So, bit 8 is forced to 0 and all other bits remain unchanged.
- Bit 8 = 0 → Push-Pull mode. To ensure the bit becomes 0, regardless of its previous value.

<img width="471" height="236" alt="image" src="https://github.com/user-attachments/assets/4c2d9fa6-e7d5-425c-921a-aa78d646f418" />

6. GPIOA->PUPDR  &= ~(0x03 << 16); // No pull-up/down: The instruction disables both pull-up and pull-down resistors on pin PB10, leaving it in a floating state (unless externally driven).

<img width="471" height="267" alt="image" src="https://github.com/user-attachments/assets/f2bc03e3-2e9b-49b0-87a1-5021a2537489" />

7. GPIOA->PUPDR  &= ~(0x03 << 16); // No pull-up/down: The instruction disables both pull-up and pull-down resistors on pin PA8, leaving it in a floating state (unless externally driven).

<img width="461" height="273" alt="image" src="https://github.com/user-attachments/assets/b44a5551-e035-4032-9184-839e97a9c080" />

TIM1 PWM configuration

8. TIM1->PSC = 84 - 1; // Timer clock assumed around 84 MHz. PSC = 83 gives 1 MHz timer frequency, 1 tick = 1 microsecond.
- <img width="221" height="52" alt="image" src="https://github.com/user-attachments/assets/cae3da96-3ea0-4c03-b154-0faebb799b71" />
- <img width="348" height="52" alt="image" src="https://github.com/user-attachments/assets/99056aeb-4836-42f1-8eed-411122e5a3f1" />
- <img width="293" height="122" alt="image" src="https://github.com/user-attachments/assets/bd306027-95b6-40fd-ac11-c85b72dacb6a" />

9. TIM1->ARR = 20000 - 1; // Servo needs 50 Hz: Period = 20 ms = 20000 us.
- <img width="221" height="34" alt="image" src="https://github.com/user-attachments/assets/2ddbfce3-3711-4562-9ee4-09af4083b579" />
- <img width="262" height="74" alt="image" src="https://github.com/user-attachments/assets/b8e963da-fc8f-40c9-8707-07a71384cede" />
- <img width="157" height="41" alt="image" src="https://github.com/user-attachments/assets/eb82e699-7e85-4045-b8c5-c388700ce60b" />

10. // PWM mode 1 on channel 1
CCMR1: It controls how channel 1 and channel 2 of the timer behave.
<img width="490" height="356" alt="image" src="https://github.com/user-attachments/assets/909bdb39-66b2-4b54-a53c-1efa6a69db00" />

TIM1->CCMR1 &= ~(0x7 << 4); For channel 1, the important bits are: Bits [6:4] → OC1M (Output Compare Mode)
- (0x7 << 4) → 0b1110000
- ~(0x7 << 4) → invert → clears bits 6, 5, 4
- &= applies the mask

Result: Bits 6, 5, 4 are set to 0, all other bits stay unchanged.

TIM1->CCMR1 |=  (0x6 << 4); Set PWM Mode 1. Output is: HIGH when counter < CCR1, LOW when counter ≥ CCR1.
- 0x6 = 0b110
- Shift left by 4
- |= sets those bits
- <img width="283" height="83" alt="image" src="https://github.com/user-attachments/assets/f51e6d74-8aaa-486e-8233-74b13aa1807c" />
- <img width="400" height="253" alt="image" src="https://github.com/user-attachments/assets/dc3b041b-d6ed-4e03-ba67-b6db03225eec" />
      
