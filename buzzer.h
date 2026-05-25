#ifndef BUZZER_H
#define BUZZER_H
// BUZZER MODULE HEADER
#include "main.h"
void Buzzer_Init(void); // INITIALIZE BUZZER GPIO
void Buzzer_On(void); // TURN BUZZER ON
void Buzzer_Off(void); // TURN BUZZER OFF
void Buzzer_Beep(uint16_t duration_ms); // GENERATE SHORT BEEP
// PARAMETER: duration_ms -> beep duration in milliseconds
void Buzzer_WrongPassword(void); // WRONG PASSWORD WARNING SOUND
void Buzzer_Alarm(void); // ALARM SOUND SEQUENCE
void Buzzer_AllSounds(void); // ALL SOUNDS TOGETHER
#endif
