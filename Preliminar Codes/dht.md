# DHT11 Temperature & Humidity Sensor - `dht.c` Description

This module implements the driver logic required to interface with a **DHT11** sensor using a single-wire digital protocol. Because the DHT11 relies on precise microsecond-level timing for its data packets, this driver bypasses standard millisecond delays by utilizing the ARM Cortex-M internal cycle counter.

---

## 📸 Hardware Reference & Pinout

To connect the DHT11 module, we used the diagrams below:

<p align="center">
  <img width="317" height="174" alt="image" src="https://github.com/user-attachments/assets/ff0b6684-d575-4857-b308-b7b7306f277a" />
  <br><br>
  <img width="160" height="131" alt="image" src="https://github.com/user-attachments/assets/28410dfe-70d6-4724-aed1-6145a2dc2b6b" />
</p>

---

## 🛠️ Hardware & Port Configuration

### DHT Description Code

* **`#define DHT_PORT GPIOC`**
<p align="center">
  <img width="251" height="278" alt="image" src="https://github.com/user-attachments/assets/a79eabcb-8b1b-41f8-bce3-90c67f667272" />
</p>

* **`#define DHT_PIN  GPIO_PIN_1`**
<p align="center">
  <img width="242" height="277" alt="image" src="https://github.com/user-attachments/assets/b45cc927-5014-4ab3-bfee-08c8600e8d87" />
</p>

---

## ⚙️ Low-Level Technical Details & Implementation

### 1. High-Resolution Microsecond Delay (`delay_us`)
```c
static void delay_us(uint32_t us)
Creates a blocking delay of us microseconds (µs). DHT11 needs timing like: 20–40 µs LOW pulses, 70 µs HIGH pulses. Normal HAL_Delay() is millisecond-based, so it’s too slow, that’s why this is needed.

uint32_t start = DWT->CYCCNT; Read current CPU cycle counter. It increments every clock cycle, acting like a high-resolution timer. Example: If CPU = 72 MHz, 72 million counts per second, 1 µs = 72 cycles.

uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000); Dynamically calculates the required CPU clock ticks based on the core frequency.

while ((DWT->CYCCNT - start) < ticks); Blocks execution until the precise amount of clock cycles has passed.

2. Enabling Peripheral Clock (RCC->AHB1ENR)
C
RCC->AHB1ENR |= (0x1 << 2); // ENABLE GPIOC CLOCK
AHB1 Peripheral Clock Enable Register controls which peripherals (like GPIO ports) receive a clock signal.

(0x1 << 2): 0x1 = binary 0001 << 2 shifts it left by 2 → becomes 0000 0100 (bit 2 set).

|= (bitwise OR assignment): Sets bit 2 without affecting other bits in the register.

3. Activating the Hardware DWT Cycle Counter
C
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; 
DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk; // ENABLE DWT COUNTER
Used to enable and start the CPU cycle counter using the ARM Cortex‑M debug/trace unit (not GPIO this time). CoreDebug represents core debug registers part of the Cortex‑M CPU.

C
/* DEMCR = Debug Exception and Monitor Control Register
TRCENA = Trace Enable bit
CoreDebug_DEMCR_TRCENA_Msk = A mask with the TRCENA bit set
CYCCNTENA = Cycle counter enable bit 
*/
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; Sets the TRCENA bit (bit 24) in DEMCR. This enables the trace and debug block, including: DWT (Data Watchpoint and Trace), ITM (Instrumentation Trace Macrocell), and the cycle counter.

DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; Sets the cycle counter enable bit. This turns on the CPU cycle counter (DWT->CYCCNT).
