# DHT11 - dht.c Description

To connect the buzzer module, we used the diagrams below. 

<img width="317" height="174" alt="image" src="https://github.com/user-attachments/assets/ff0b6684-d575-4857-b308-b7b7306f277a" />

<img width="160" height="131" alt="image" src="https://github.com/user-attachments/assets/28410dfe-70d6-4724-aed1-6145a2dc2b6b" />

DHT Description Code

- #define DHT_PORT GPIOC

<img width="251" height="278" alt="image" src="https://github.com/user-attachments/assets/a79eabcb-8b1b-41f8-bce3-90c67f667272" />

- #define DHT_PIN  GPIO_PIN_1

<img width="242" height="277" alt="image" src="https://github.com/user-attachments/assets/b45cc927-5014-4ab3-bfee-08c8600e8d87" />

- static void delay_us(uint32_t us) //MICROSECOND DELAY

Creates a blocking delay of us microseconds (µs). DHT11 needs timing like: 20–40 µs LOW pulses, 70 µs HIGH pulses.
Normal HAL_Delay() is millisecond-based, so it’s too slow, that’s why this is needed.

1. uint32_t start = DWT->CYCCNT: Read current CPU cycle counter. It increments every clock cycle, cts like a
high-resolution timer. Example: If CPU = 72 MHz, 72 million counts per second, 1 µs = 72 cycles.

2. uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000):

3. while ((DWT->CYCCNT - start) < ticks):

- RCC->AHB1ENR |= (0x1 << 2); // ENABLE GPIOC CLOCK

AHB1 Peripheral Clock Enable Register, this register controls which peripherals (like GPIO ports) receive a clock signal.
(0x1 << 2): 0x1 = binary 0001 << 2 shifts it left by 2 → becomes 0000 0100 (bit 2 set).
|= (bitwise OR assignment): Sets bit 2 without affecting other bits in the register.

