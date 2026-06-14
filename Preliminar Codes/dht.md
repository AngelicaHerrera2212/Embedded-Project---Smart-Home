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
