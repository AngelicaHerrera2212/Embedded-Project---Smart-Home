# DC FAN - fan_control.c Description

<img width="536" height="374" alt="image" src="https://github.com/user-attachments/assets/33e3eb97-cfb1-441b-a8f4-380add812709" />

Fan Control Code Description

- #define FAN_TARGET_TEMP 22.0f // Desired temperature in °C: Threshold

- #define FAN_KP 25.0f // Proportional gain: It determines how strongly the fan reacts to temperature error.

<img width="152" height="34" alt="image" src="https://github.com/user-attachments/assets/a455905f-e712-48ea-bfdf-8868b98520d8" />
<img width="99" height="34" alt="image" src="https://github.com/user-attachments/assets/8c4a8de1-0f6a-4840-ba14-26cdb567317e" />
<img width="146" height="72" alt="image" src="https://github.com/user-attachments/assets/254334ee-d3c1-4ad3-a78c-47de4cb0e489" />

- #define FAN_DEADBAND 0.5f // Ignore small errors: 

- #define FAN_MIN_PWM 30 // Minimum PWM to start fan: 

- #define FAN_MAX_PWM 100 // Maximum PWM:

- #define DHT_READ_PERIOD_MS 2000 // DHT should not be read too fast: 

