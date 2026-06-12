#include "buzzer.h"
/* BUZZER MODULE
*
* GPIO ASSIGNMENT: PB10 -> Active Buzzer
*
* FUNCTIONS:
* - Initialize buzzer GPIO
* - Turn buzzer ON/OFF
* - Generate short beep
* - Generate alarm sequence
*/
void Buzzer_Init(void)
{
   //ENABLE GPIOB CLOCK
   RCC->AHB1ENR |= (0x1 << 1);
   /*CONFIGURE PB10 AS OUTPUT
    * MODER:
    * 00 = INPUT
    * 01 = OUTPUT
    * 10 = ALTERNATE FUNCTION
    * 11 = ANALOG
    *
    * PB10 -> bits [21:20]
    */
   GPIOB->MODER &= ~(0x03 << 20);   // Clear bits [21:20] - Apply Mask and force to 0
   GPIOB->MODER |=  (0x01 << 20);   // Set PB10 as OUTPUT - OR
   //OUTPUT TYPE: 0 = PUSH-PULL, 1 = OPEN-DRAIN
   GPIOB->OTYPER &= ~(0x1 << 10);
   //NO PULL-UP / PULL-DOWN
   GPIOB->PUPDR &= ~(0x03 << 20);
   //INITIAL STATE = OFF
   GPIOB->ODR &= ~(0x1 << 10);
}
//*TURN BUZZER ON
void Buzzer_On(void)
{
   GPIOB->ODR |= (0x1 << 10);
}
//TURN BUZZER OFF
void Buzzer_Off(void)
{
   GPIOB->ODR &= ~(0x1 << 10);
}
//SHORT BEEP
void Buzzer_Beep(uint16_t duration_ms)
{
   Buzzer_On();
   HAL_Delay(duration_ms);
   Buzzer_Off();
}
// WRONG PASSWORD SOUND
void Buzzer_WrongPassword(void)
{
   Buzzer_Beep(250);
   HAL_Delay(100);
   Buzzer_Beep(250);
}
// ALARM SOUND
void Buzzer_Alarm(void)
{
   for(int i = 0; i < 5; i++)
   {
       Buzzer_On();
       HAL_Delay(300);
       Buzzer_Off();
       HAL_Delay(300);
   }
}
// ALL SOUNDS
void Buzzer_AllSounds(void)
{
    // Sound 1: simple short beep
    Buzzer_Beep(150);
    HAL_Delay(200);

    // Sound 2: wrong password pattern
    Buzzer_WrongPassword();
    HAL_Delay(300);

    // Sound 3: alarm pattern
    Buzzer_Alarm();
    HAL_Delay(300);
}
