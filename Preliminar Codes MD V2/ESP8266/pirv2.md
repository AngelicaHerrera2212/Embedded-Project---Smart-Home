# PIR HC-SR501 - pir.c Description

<p align="center">
  <img width="364" height="179" alt="image" src="https://github.com/user-attachments/assets/609acd50-a906-4a0a-8536-368f572a6d73" />
</p>

<p align="center">
  <img width="305" height="434" alt="image" src="https://github.com/user-attachments/assets/f39f9a1f-e96d-4b3b-95ff-27c997ed338b" />
</p>

---

## Technical Specifications & Pinout Connections

The HC-SR501 Passive Infrared (PIR) motion sensor is integrated directly into the **STM32 Nucleo-F446RE** development board to provide real-time occupancy detection inside the automated room. 

### Pin Mapping Table

| PIR Sensor Pin | Nucleo Connector Pin | MCU Internal Port / Pin | Signal Type | Physical Location / Context |
| :--- | :--- | :--- | :--- | :--- |
| **VCC** | **CN6-5** (or CN7-18) | **5V Power (from USB)** | Power Supply | Connected to the 5V rail of the MB-102 power module. |
| **OUT** | **CN8-37** | **PC0** | **Digital Input** | Motion detection signal (Inside House tracking). |
| **GND** | **CN6-7** (or CN8-11) | **GND** | Ground Reference | Tied to the system-wide common ground. |

---

## Hardware Implementation Details

* **5V Operating Voltage:** Unlike most components running natively at 3.3V, the HC-SR501 **VCC** pin requires a stable **5V** power rail, supplied via the Nucleo's shared USB power bus or the MB-102 power module.
* **Logic Level Compatibility:** Although powered by 5V, the sensor's **OUT** pin outputs a **3.3V** logic high signal when motion is detected. This makes it perfectly safe to connect directly to the STM32 **PC0** pin as a standard **Digital Input** without needing any level shifter or voltage divider.
* **Common Ground Principle:** The **GND** pin must be tied to the absolute common ground of the circuit. Sharing a common ground across the STM32, ESP8266, sensors, and external power supplies is mandatory to prevent electrical noise and false motion triggers.

---

## Firmware Operational Logic (`pir.c`)

When configuring the GPIO peripheral in your development environment for pin **PC0**, the firmware reads and processes the signal state based on the following logic:

* **Logic Low State (`LOW` / `0`):** No motion is detected in the environment. The sensor output sits at 0V. The STM32 parses this state and appends `MOTION=0` to the status string sent over the UART bus to the ESP8266 web server.
* **Logic High State (`HIGH` / `1`):** The sensor detects infrared radiation changes (human or animal movement), jumping immediately to 3.3V. The STM32 micro-controller catches this transition on pin **PC0**, updates its status register to `1`, and builds the updated data packet (`MOTION=1`) to flag the motion state on the mobile application dashboard.
