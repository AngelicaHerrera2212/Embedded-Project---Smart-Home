# LEDs Module - `leds.c` Description

This module handles the control logic, state management, and visual feedback indicators using LEDs connected to the microcontroller. It provides a clean abstraction layer to turn indicators on/off, toggle states, or handle visual patterns based on system events.

---

## 📸 Component Reference

<p align="center">
  <img width="223" height="90" alt="image" src="https://github.com/user-attachments/assets/88b3c9ae-5ae7-4ae1-8d8a-666e35f07270" />
  <br><br>
  <img width="175" height="131" alt="Screenshot 2026-05-28 012453" src="https://github.com/user-attachments/assets/4ce0d69e-e102-440b-8dcd-2007c0983ae2" />
</p>

---

## ⚙️ Features & Functionality

* **Abstraction Layer:** Decouples direct GPIO register manipulation or HAL calls from the main application logic.
* **State Indicators:** Provides real-time visual diagnostics (e.g., connection status, data transmission, system errors, or idling states).
* **Non-blocking Visuals:** Designed to work efficiently alongside hardware timers or main execution loops without stalling critical operations.

## 🛠️ Hardware Integration

| Component | MCU Pin Assignment | Logic Type | Description |
| :--- | :--- | :--- | :--- |
| **LED Indicator(s)** | *[e.g., PA5 / User LED]* | Active High / Low | Visual output feedback for system routines. |

> 💡 **Tip:** Update the pin assignment row above with the exact GPIO configurations used in your implementation (e.g., the specific STM32 pins assigned to your status LEDs).
