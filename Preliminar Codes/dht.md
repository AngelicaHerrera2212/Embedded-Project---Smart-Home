Markdown
# DHT11 Temperature & Humidity Sensor - `dht.c` Description

This module implements the driver logic required to interface with a **DHT11** sensor using a single-wire digital protocol. Because the DHT11 relies on precise microsecond-level timing for its data packets, this driver bypasses standard millisecond delays by utilizing the ARM Cortex-M internal cycle counter.

---

## 📸 Hardware Reference & Pinout

<p align="center">
  <img width="317" height="174" alt="DHT11 Sensor Connection" src="https://github.com/user-attachments/assets/ff0b6684-d575-4857-b308-b7b7306f277a" />
  <br><br>
  <img width="160" height="131" alt="DHT11 Pin Interface" src="https://github.com/user-attachments/assets/28410dfe-70d6-4724-aed1-6145a2dc2b6b" />
</p>

---

## 🛠️ Hardware & Port Configuration

The sensor data line is mapped to **GPIOC** on **Pin 1** (`PC1`). 

### Hardware Mapping Definitions
```c
#define DHT_PORT  GPIOC
#define DHT_PIN   GPIO_PIN_1
⚙️ Low-Level Technical Details & Implementation
1. High-Resolution Microsecond Delay (delay_us)
The DHT11 protocol demands highly precise timing windows (e.g., 20–40 µs LOW pulses, 70 µs HIGH pulses). Standard HAL_Delay() is millisecond-based and too slow. To achieve microsecond precision, we utilize the DWT (Data Watchpoint and Trace) peripheral's cycle counter (CYCCNT).

C
static void delay_us(uint32_t us)
uint32_t start = DWT->CYCCNT; Reads the current CPU cycle counter. It acts as a high-resolution timer. For example, on a 180 MHz clock, 1 µs equals 180 clock cycles.

uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000); Dynamically calculates the required CPU clock ticks needed for the requested microseconds based on the current system core clock speed.

while ((DWT->CYCCNT - start) < ticks); Blocks execution in a tight loop until the precise amount of clock cycles has passed.

2. Enabling Peripheral Clock (RCC->AHB1ENR)
Before interacting with GPIOC, its peripheral clock must be enabled in the hardware register.

C
RCC->AHB1ENR |= (0x1 << 2); // ENABLE GPIOC CLOCK
AHB1 Peripheral Clock Enable Register: Controls the clock signals sent to the GPIO ports.

(0x1 << 2): Shifts 0x01 left by 2 positions, targeting bit 2 (which corresponds directly to GPIOC).

|= (Bitwise OR assignment): Safely sets bit 2 to 1 without clearing or altering any other peripheral settings in the register.

3. Activating the Hardware DWT Cycle Counter
To make delay_us work, the ARM Cortex-M debug/trace system and cycle counter hardware block must be manually turned on:

C
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; 
DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk; // ENABLE DWT COUNTER
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; Sets the TRCENA (Trace Enable) bit 24 in the Debug Exception and Monitor Control Register. This powers on the trace sub-blocks, including the DWT engine.

DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; Sets the CYCCNTENA bit in the DWT Control register, turning on the hardware cycle counter (DWT->CYCCNT) so it begins incrementing on every CPU clock edge.
