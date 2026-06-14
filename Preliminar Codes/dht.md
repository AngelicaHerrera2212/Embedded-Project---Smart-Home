Markdown
# DHT11 Temperature & Humidity Sensor - `dht.c` Description

To interface with the DHT11 sensor module, the physical connections and configuration follow the implementation parameters detailed below.

<p align="center">
  <img width="317" height="174" alt="image" src="https://github.com/user-attachments/assets/ff0b6684-d575-4857-b308-b7b7306f277a" />
</p>

<p align="center">
  <img width="160" height="131" alt="image" src="https://github.com/user-attachments/assets/28410dfe-70d6-4724-aed1-6145a2dc2b6b" />
</p>

## 🛠️ Hardware Mapping Definitions

The hardware register boundaries and signal pins are explicitly mapped as follows:

- **`#define DHT_PORT GPIOC`**

<p align="center">
  <img width="251" height="278" alt="image" src="https://github.com/user-attachments/assets/a79eabcb-8b1b-41f8-bce3-90c67f667272" />
</p>

- **`#define DHT_PIN  GPIO_PIN_1`**

<p align="center">
  <img width="242" height="277" alt="image" src="https://github.com/user-attachments/assets/b45cc927-5014-4ab3-bfee-08c8600e8d87" />
</p>

---

## ⚙️ Low-Level Driver Implementation

### 1. High-Resolution Microsecond Delay (`delay_us`)

```c
static void delay_us(uint32_t us)
This function creates a blocking hardware delay measured in microseconds (µs). Precision timing is critical for the DHT11 single-wire protocol, which communicates using transient signal windows (e.g., 20–40 µs LOW pulses followed by 70 µs HIGH pulses). The native HAL_Delay() utility is millisecond-based and entirely inadequate for these demanding timing constraints.

uint32_t start = DWT->CYCCNT;
Captures the current snapshot of the CPU cycle counter. This counter increments on every individual clock edge, acting as a highly deterministic, high-resolution hardware timer. For example, if the core CPU runs at 72 MHz, the counter clocks 72 million times per second, establishing a resolution where 1 µs precisely equals 72 cycles.

uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);
Dynamically scales and calculates the exact threshold of CPU clock cycles required to match the target microsecond duration based on the active system core frequency.

while ((DWT->CYCCNT - start) < ticks);
Polls the hardware register continuously in a tight execution loop until the calculated clock cycle delta has elapsed.

2. Peripheral Clock Initialization
C
RCC->AHB1ENR |= (0x1 << 2); // ENABLE GPIOC CLOCK
The AHB1 Peripheral Clock Enable Register gating controls the distribution of power and clock lines to individual peripheral blocks, such as GPIO configurations.

(0x1 << 2): Left-shifts binary 0001 by two positions, evaluating to 0000 0100 (directly asserting bit position 2, which corresponds to GPIOC gating).

|= (Bitwise OR Assignment): Safely forces bit 2 high to wake up the GPIOC bus without clearing or altering adjacent peripheral configuration statuses inside the register.

3. Core Debug & DWT Hardware Counter Activation
C
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; 
DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk; // ENABLE DWT COUNTER
To leverage the high-resolution hardware cycle counter, the developer must explicitly enable and power up the internal CPU debug, tracing, and watchpoint macrocells inside the ARM Cortex-M architecture.

C
/* * DEMCR: Debug Exception and Monitor Control Register
 * TRCENA: Global Trace Enable Bit
 * CoreDebug_DEMCR_TRCENA_Msk: Standard mask targeting the TRCENA bit
 * CYCCNTENA: Cycle Counter Enable Bit
 */
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
Sets the global TRCENA bit (Bit 24) inside the processor's DEMCR register. This un-gates the debug/trace infrastructure, waking up underlying modules including the DWT (Data Watchpoint and Trace) engine and ITM (Instrumentation Trace Macrocell).

DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
Asserts the explicit cycle counter activation flag inside the DWT control layout, prompting the hardware cycle register (DWT->CYCCNT) to begin incrementing actively.
```c

<p align="center">
  <img width="425" height="331" alt="image" src="https://github.com/user-attachments/assets/e9c65b3b-f3d7-4406-b8d6-77d8de81e0da" />
</p>
