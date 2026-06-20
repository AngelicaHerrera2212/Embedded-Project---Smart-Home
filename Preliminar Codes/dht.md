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

Creates busy‑wait microsecond delay using the DWT cycle counter of the ARM Cortex‑M.DHT11 needs timing like: 20–40 µs LOW pulses, 70 µs HIGH pulses. Normal HAL_Delay() is millisecond-based, so it’s too slow, that’s why this is needed.

<img width="332" height="84" alt="image" src="https://github.com/user-attachments/assets/63f62a01-3916-49bf-8adb-cad3d243833a" />

1. uint32_t start = DWT->CYCCNT: DWT->CYCCNT is a hardware counter that increments every CPU clock cycle. It marks the current time in CPU cycles. It increments every clock cycle, counts like a high-resolution timer. Example: If CPU = 72 MHz, 72 million counts per second, 1 µs = 72 cycles.

2. uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000): HAL_RCC_GetHCLKFreq() returns the CPU frequency (e.g., 84 MHz) dividing by 1,000,000 gives cycles per microsecond. Tells us how many CPU cycles correspond to the requested delay.

3. while ((DWT->CYCCNT - start) < ticks): This loop keeps running until enough cycles have passed. DWT->CYCCNT - start = how many cycles have elapsed. When elapsed cycles ≥ required cycles → exit the loop.

void DHT_Init(void)

- RCC->AHB1ENR |= (0x1 << 2); // ENABLE GPIOC CLOCK

AHB1 Peripheral Clock Enable Register, this register controls which peripherals (like GPIO ports) receive a clock signal.
(0x1 << 2): 0x1 = binary 0001 << 2 shifts it left by 2 → becomes 0000 0100 (bit 2 set).
|= (bitwise OR assignment): Sets bit 2 without affecting other bits in the register.

<img width="425" height="331" alt="image" src="https://github.com/user-attachments/assets/e9c65b3b-f3d7-4406-b8d6-77d8de81e0da" />

- CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk // ENABLE DWT COUNTER, used for microsecond timing

This part turns on the hardware needed for delay_us() to work. Used to enable and start the CPU cycle counter using the ARM Cortex‑M debug/trace unit (not GPIO this time).

<img width="272" height="38" alt="image" src="https://github.com/user-attachments/assets/7d4af327-8bbb-478f-a012-e747c531e570" />

/*DEMCR = Debug Exception and Monitor Control Register
TRCENA = Trace Enable bit
CoreDebug_DEMCR_TRCENA_Msk = A mask with the TRCENA bit set
CYCCNTENA = Cycle counter enable bit*/

1. CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk: Sets the TRCENA bit (bit 24) in DEMCR. This enables the trace and debug block, including: DWT (Data Watchpoint and Trace), ITM (Instrumentation Trace Macrocell), cycle counter. Using |= sets that bit to 1 while leaving other bits unchanged. 

2. DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk. Enable the CPU cycle counter. DWT->CTRL is the control register of the Data Watchpoint and Trace unit. DWT_CTRL_CYCCNTENA_Msk is the bit that turns on the CYCCNT counter. Once set, DWT->CYCCNT starts incrementing every CPU clock cycle, which your delay_us() function uses to measure time precisely.

static void DHT_SetOutput(void) // FUNCTION TO CONFIGURE PC1 AS OUTPUT. Used during DHT START SIGNAL

- GPIOC->MODER &= ~(0x03 << 2); // Clear bits [3:2] - Apply Mask and force to 0. GPIOC->MODER |=  (0x01 << 2); // Set PA8 as output - use OR bitwise compares to turn specifically this bit on (it turns ON the bit without affecting other bits), finally set the bits to 01.

<img width="448" height="277" alt="image" src="https://github.com/user-attachments/assets/d183d5ba-3a35-412c-854f-ccfbd2313bc2" />

- GPIOC->OTYPER &= ~(0x1 << 1); // Push-pull output: Controls the output type of pins, each pin uses 1 bit only.

1. 0x1 << 1, creates the value 0b10. This selects bit 1 (which corresponds to PC1).
2. ~(0b10), becomes 0b111111...101. This is a mask with bit 1 = 0 and all other bits = 1.
3. GPIOC->OTYPER &= mask, use AND with a mask that has a 0 in bit 1 forces that bit to 0. All other bits stay unchanged because AND with 1 keeps the value.
   
<img width="458" height="223" alt="image" src="https://github.com/user-attachments/assets/6ea61dfa-c40d-4bca-b637-d91e1c7b22cc" />

- GPIOC->PUPDR &= ~(0x03 << 2); //NO PULL-UP / PULL-DOWN: The instruction disables both pull-up and pull-down resistors on pin PB6, leaving it in a floating state (unless externally driven). Because 0x03=0b11, with the mask and the AND set the bits 00.
   
<img width="446" height="267" alt="image" src="https://github.com/user-attachments/assets/839a460b-96c3-4a7c-bd00-7499aa43d905" />

static void DHT_SetInput(void) // FUNCTION TO CONFIGURE PC1 AS INPUT. Used during DATA RECEPTION

- GPIOC->MODER &= ~(0x03 << 2); // INPUT MODE:

1. First, 0x03 in binary is 0b11. Two bits set.
2. 0x03 << 2 shifts those bits to positions 3:2
3. ~(0x03 << 2) inverts the mask.
4. &= clears those two bits (set them to 0). Remember: AND with 0 → forces the bit to 0. AND with 1 → keeps the bit unchanged

<img width="463" height="284" alt="image" src="https://github.com/user-attachments/assets/27b2f695-3cf8-4544-8057-879ba96d2134" />

- GPIOC->PUPDR &= ~(0x03 << 2); //ENABLE PULL-UP: selects those two bits, creates a mask with 0s in bits [3:2]. &= clears those bits to 00.

GPIOC->PUPDR |=  (0x01 << 2); 0x01 = 01 in binary (pull‑up), Shifted to bits [3:2] to applie to PC1. |= writes 01 into those bits.
   
<img width="458" height="269" alt="image" src="https://github.com/user-attachments/assets/9ea1f87d-e4ef-4c42-9605-a696df4f588e" />

Read DHT Function:

The function returns 1 on success, 0 on error. Writes the measured values into *temperature and *humidity.

<img width="295" height="25" alt="image" src="https://github.com/user-attachments/assets/7445eb4a-b0b4-475a-91c0-8f28b5f7a15b" />

This part is raw data buffer: DHT11 sends 40 bits = 5 bytes: data[0] = humidity integer, data[1] = humidity decimal, data[2] = temperature integer, data[3] = temperature decimal, data[4] = checksum.

<img width="131" height="13" alt="image" src="https://github.com/user-attachments/assets/296de5e4-9474-48ac-9891-c92fd58188ad" />

This part is timeout counter, used in loops to avoid getting stuck forever if the sensor doesn’t respond.

<img width="99" height="13" alt="image" src="https://github.com/user-attachments/assets/7c39330c-d8ac-4a26-8b32-03397732a213" />

DHT_SetOutput(); // Now this function send the Start signal (MCU → DHT).****

- GPIOC->ODR &= ~(0x1 << 1); HAL_Delay(18); // Drive PC1 LOW → this is the start signal to the DHT11. The DHT11 expects the line to be pulled low for at least 18 ms. 

- GPIOC->ODR |= (0x1 << 1); // Then Release the line by driving it HIGH again.

-  delay_us(30); // After the start signal, the MCU must wait a short time before listening for the sensor’s response.

<img width="154" height="73" alt="image" src="https://github.com/user-attachments/assets/b8601cd3-9d8a-433c-8db3-4042ce33d652" />

Next part: Switch to input and wait for DHT response. ****

<img width="137" height="25" alt="image" src="https://github.com/user-attachments/assets/61cd7e6c-1c4a-4b3b-b3c8-e5fbe9015e7f" />

Perform a (LOW–HIGH–LOW–HIGH pattern) before the sensor starts sending the 40 data bits.****


Now wait while the line is HIGH. The DHT11 should pull the line LOW to start its response. If it stays HIGH too long → timeout → return error.

<img width="182" height="81" alt="image" src="https://github.com/user-attachments/assets/3d67aeb9-8720-499b-85cb-f4deee089345" />

Now wait while the line is LOW. The DHT11 then pulls it HIGH as part of its response sequence. Again, timeout if it doesn’t change.

<img width="186" height="70" alt="image" src="https://github.com/user-attachments/assets/7496e627-6e7d-40f5-bbe2-02aa3ec779ba" />

Finally, wait while the line is HIGH again. After this handshake (LOW–HIGH–LOW–HIGH pattern), the sensor starts sending the 40 data bits.

<img width="185" height="71" alt="image" src="https://github.com/user-attachments/assets/e4c4c3f8-4f05-428b-92df-a04064da3da8" />

Next part: Loop over all 40 bits sent by the DHT11.*****

<img width="164" height="37" alt="image" src="https://github.com/user-attachments/assets/7e7b5027-8186-4672-9f1d-a76e9f4f912e" />

For each bit, first wait for the line to go HIGH. The DHT11 encodes bits as: A short LOW, then A HIGH pulse whose length indicates 0 or 1. Then wait ~40 µs after the rising edge. At this time, the line will still be HIGH for a ‘1’ bit, but will already have gone LOW for a ‘0’ bit (because 0 has a shorter HIGH pulse).

<img width="200" height="83" alt="image" src="https://github.com/user-attachments/assets/d0940cec-04d7-4aa6-ac76-d81f5ae8ce15" />

Next: sample the line: If it’s still HIGH → treat this bit as 1. If it’s LOW → bit is 0. data[i / 8] selects which byte (0–4). (7 - (i % 8)) sets bits from MSB to LSB inside each byte.

<img width="232" height="47" alt="image" src="https://github.com/user-attachments/assets/1999e94d-36cf-4dca-ac94-9772d4841d85" />

Then wait for the line to go LOW again, marking the end of this bit’s HIGH pulse and preparing for the next bit.

<img width="182" height="73" alt="image" src="https://github.com/user-attachments/assets/988bf5d9-8d14-4b90-9ecd-db00000d85c2" />

Perform checksum validation.****

After reading all 40 bits, the sensor has filled this array:. DHT11 checksum rule: checksum = (byte0 + byte1 + byte2 + byte3) & 0xFF. If the sum of the first 4 bytes doesn’t match data[4], the frame is invalid → return error. (Lenght is 2^8 = 256, uint8)

<img width="347" height="21" alt="image" src="https://github.com/user-attachments/assets/e74c9267-eafb-4f2f-9087-61ea34664126" />

<img width="355" height="61" alt="image" src="https://github.com/user-attachments/assets/5a0d1804-3fb0-4099-ad6f-5cd4768408ce" />

Store values in output variables.****

- Humidity integer part → data[0]
- Temperature integer part → data[2]
- Ignore decimal bytes (data[1], data[3]) since DHT11 usually uses only integer resolution.

<img width="132" height="35" alt="image" src="https://github.com/user-attachments/assets/bd2d7d89-d7d9-4921-b7e3-04d101b0325b" />

Finally, return 1 to indicate success.


















