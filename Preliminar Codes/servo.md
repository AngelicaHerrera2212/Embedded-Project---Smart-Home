# SERVO SG90 - servo.c Description

To connect the servo motor, we used the diagrams below.

- <img width="289" height="208" alt="image" src="https://github.com/user-attachments/assets/a53ea6ee-5289-4fc6-aec2-9331f157c9ab" />
- <img width="314" height="454" alt="image" src="https://github.com/user-attachments/assets/b4ebc1ce-713e-4e2f-a820-40bdbf645e27" />

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

1. RCC->AHB1ENR |= (0x1 << 0); // Enable GPIOA clock
    


