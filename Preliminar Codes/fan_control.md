# DC FAN - `fan_control.c` Description

This driver module manages the operation, speed control, and state machine transitions of a DC cooling fan using the microcontroller's hardware peripherals. It translates system commands into precise hardware actions to handle thermal management or ventilation requirements.

---

## 📸 Technical Reference Diagrams

<p align="center">
  <img width="404" height="305" alt="DC Fan Schematic/Logic" src="https://github.com/user-attachments/assets/76f389ee-2b5c-4e74-a2f1-55362ae46f04" />
  <br><br>
  <img width="394" height="307" alt="Control Configuration" src="https://github.com/user-attachments/assets/5c6087ac-f8c6-468f-a910-40867ea8844f" />
  <br><br>
  <img width="536" height="354" alt="Timing/Waveform Diagram" src="https://github.com/user-attachments/assets/9c5f8208-d90b-4644-8197-9d62f5718ce1" />
  <br><br>
  <img width="536" height="374" alt="Hardware Setup" src="https://github.com/user-attachments/assets/33e3eb97-cfb1-441b-a8f4-380add812709" />
</p>

---

## ⚙️ Features & Functionality

* **Speed Control:** Implements speed management (e.g., via discrete states or Pulse Width Modulation - PWM) to regulate airflow dynamically.
* **State Machine Integration:** Seamlessly responds to system events such as "Idling", "Active Cooling", or safety-related emergency stops.
* **Hardware Interfacing:** Managed via an external driving circuit (such as a transistor/MOSFET or relay stage) to safely isolate the microcontroller from the higher current drawn by the DC motor.

## 🛠️ Hardware & Peripheral Mapping

| Component | Signal Type | Control Hardware | Description |
| :--- | :--- | :--- | :--- |
| **DC Fan** | Digital Out / PWM | *[e.g., TIMx_CHy or GPIO]* | Main actuator for environmental cooling. |
| **Driver Circuit** | Gate/Base Trigger | N-Channel MOSFET / Transistor | Interface to step up logic voltage (3.3V) to fan operating voltage. |

> 💡 **Implementation Note:** Ensure that the timer channels or specific GPIO pins configured inside your hardware initialization files perfectly match the drive signal pins specified in this documentation.
