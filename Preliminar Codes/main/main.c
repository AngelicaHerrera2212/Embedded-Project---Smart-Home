/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "buzzer.h"
#include "dht.h"
#include "pir.h"
#include "leds.h"
#include "servo.h"
#include "fan_control.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// Global sensor variables
float temperature = 0.0f;
float humidity = 0.0f;
static float current_temperature = 0.0f;
static float current_humidity = 0.0f;
uint8_t fan_pwm_percent = 0;
// PID state
static float pid_integral = 0.0f;
static float previous_error = 0.0f;

// UART Communication variables

/* Stores the last byte received from ESP8266. UART interrupt receives one byte
at a time. */
uint8_t rx_byte;
// Temporary buffer used to store incoming characters.
char rx_buffer[64];
// Stores the complete command after '\n' is received. Example:command = "LIGHT:ON"
char command[64];

// Current position inside rx_buffer. Increases every time a new character arrives.
uint8_t rx_index = 0;
/* Flag set by UART interrupt.
* 0 -> no complete command available
* 1 -> command ready to be processed */
volatile uint8_t command_ready = 0;

// Access Control

/* Counts consecutive wrong password attempts.
If wrong_attempts reaches 4: activate buzzer alarm */
uint8_t wrong_attempts = 0;
/* Master password used to open the door. */
char correct_password[] = "1234";

// Lighting Control Variables

/* Lighting operating mode.
* 1 -> AUTO mode: PIR controls LEDs
* 0 -> MANUAL mode: App controls LEDs */
uint8_t light_auto_mode = 1;
/* Desired LED state in MANUAL mode.
* 1 -> LEDs ON
* 0 -> LEDs OFF */
uint8_t manual_light_state = 0;

// System Status Variables

/* Current door state.
* 1 -> OPEN
* 0 -> CLOSED */
uint8_t door_open = 0;
/* Alarm state.
* 1 -> alarm active
* 0 -> alarm inactive */
uint8_t alarm_active = 0;
/* Motion detection status. Updated by PIR module.
* 1 -> motion detected
* 0 -> no motion */
uint8_t motion_detected = 0;

// Periodic Task Timers

/* Timestamp of last status transmission.
* Used to send system status every few seconds
* without blocking the processor. */
uint32_t last_status_time = 0;
/* Timestamp of last PIR evaluation.
* Used to periodically check motion detection
* without using HAL_Delay(). */
uint32_t last_pir_time = 0;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void Process_Command(void);
void Send_Status(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* Initialize external modules */
  Buzzer_Init();   // PB10 = Buzzer output
  DHT_Init();      // PC1 = DHT11/DHT22 data pin
  PIR_Init();	   // PC2 = PIR Sensor
  LEDs_Init();	   // PC3 = LED1, PC4 = LED2, PC5 = LED3, PB0 = LED4, PB1 = LED5
  Servo_Init();    // PA8 = Servo Signal
  Servo_CloseDoor();
  door_open = 0;
  FanControl_Init(); // PB6 = Fan PWM

  /* Start UART interrupt reception. STM32 receives one byte at a time from ESP8266. */
  HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  /* Infinite loop */
  while (1)
  {
	  //Servo_SetAngle(180);
	  //HAL_Delay(1500);
	  //Servo_SetAngle(0);
	  //HAL_Delay(1500);
	  //DHT_Read(&temperature, &humidity);
	  // TESTING DHT, BUZZER, LEDS AND PIR
	  /* CONDITIONS:
       * If: temperature > 22°C AND motion detected
       * THEN: turn ON all LEDs AND activate buzzer sounds
       */

      /*if (DHT_Read(&temperature, &humidity))
      {
          if ((temperature > 22.0f) && PIR_RawRead())
          {
              LEDs_On();
              Buzzer_AllSounds();
          }
          else
          {
              LEDs_Off();
          }
     }*/


      // TESTING SERVO
        /*Servo_SetAngle(0);
        HAL_Delay(1500);

        Servo_SetAngle(45);
        HAL_Delay(1500);

        Servo_SetAngle(90);
        HAL_Delay(1500);

        Servo_SetAngle(120);
        HAL_Delay(1500);

        Servo_SetAngle(180);
        HAL_Delay(1500);*/


      // TESTING FAN CONTROL
        /*FanControl_Update();
        HAL_Delay(100);*/

	  // FINAL WHILE LOOP
	     // 1. Process command received from ESP8266/app
	     if (command_ready)
	     {
	         command_ready = 0;
	         Process_Command();
	     }
	     // 2. Update DHT + fan PWM control
	     FanControl_Update();
	     // 3. Check PIR every 200 ms
	     if (HAL_GetTick() - last_pir_time >= 200)
	     {
	         last_pir_time = HAL_GetTick();
	         motion_detected = PIR_RawRead();
	     }
	     // 4. Update LEDs depending on mode
	     if (light_auto_mode)
	     {
	         if (motion_detected)
	         {
	             LEDs_On();
	         }
	         else
	         {
	             LEDs_Off();
	         }
	     }
	     else
	     {
	         if (manual_light_state)
	         {
	             LEDs_On();
	         }
	         else
	         {
	             LEDs_Off();
	         }
	     }
	     // 5. Send status every 2 seconds
	     /*if (HAL_GetTick() - last_status_time >= 2000)
	     {
	         last_status_time = HAL_GetTick();
	         Send_Status();
	     }*/


     /* USER CODE BEGIN 3*/
     HAL_Delay(10);
     /* USER CODE END 3 */
  }
  /* USER CODE BEGIN WHILE */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}


/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

{
    if (huart->Instance == USART1)
    {
        if (rx_byte == '\n')
        {
            rx_buffer[rx_index] = '\0';
            strcpy(command, rx_buffer);
            rx_index = 0;
            command_ready = 1;
        }
        else if (rx_byte != '\r')
        {
            if (rx_index < sizeof(rx_buffer) - 1)
            {
                rx_buffer[rx_index++] = rx_byte;
            }
        }
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}

void Process_Command(void)
{
   if (strncmp(command, "PASS:", 5) == 0)
   {
       char *received_password = command + 5;
       if (strcmp(received_password, correct_password) == 0)
       {
           wrong_attempts = 0;
           door_open = 1;
           Servo_OpenDoor();
           alarm_active = 0;
           HAL_UART_Transmit(&huart1,
                             (uint8_t*)"ACK:ACCESS_GRANTED\n",
                             strlen("ACK:ACCESS_GRANTED\n"),
                             100);
       }
       else
       {
           wrong_attempts++;
           Buzzer_WrongPassword();
           alarm_active = 1;
           if (wrong_attempts >= 4)
           {
               alarm_active = 1;
               Buzzer_Alarm();
               wrong_attempts = 0;
           }
           HAL_UART_Transmit(&huart1,
                             (uint8_t*)"ACK:ACCESS_DENIED\n",
                             strlen("ACK:ACCESS_DENIED\n"),
                             100);
       }
   }
   else if (strcmp(command, "DOOR:CLOSE") == 0)
   {
       Servo_CloseDoor();
       door_open = 0;
       HAL_UART_Transmit(&huart1,
                         (uint8_t*)"ACK:DOOR_CLOSED\n",
                         strlen("ACK:DOOR_CLOSED\n"),
                         100);
   }
   else if (strcmp(command, "LIGHT_MODE:AUTO") == 0)
   {
       light_auto_mode = 1;
       HAL_UART_Transmit(&huart1,
                         (uint8_t*)"ACK:LIGHT_AUTO\n",
                         strlen("ACK:LIGHT_AUTO\n"),
                         100);
   }
   else if (strcmp(command, "LIGHT_MODE:MANUAL") == 0)
   {
       light_auto_mode = 0;
       HAL_UART_Transmit(&huart1,
                         (uint8_t*)"ACK:LIGHT_MANUAL\n",
                         strlen("ACK:LIGHT_MANUAL\n"),
                         100);
   }
   else if (strcmp(command, "LIGHT:ON") == 0)
   {
       manual_light_state = 1;
       if (!light_auto_mode)
       {
           LEDs_On();
       }
       HAL_UART_Transmit(&huart1,
                         (uint8_t*)"ACK:LIGHT_ON\n",
                         strlen("ACK:LIGHT_ON\n"),
                         100);
   }
   else if (strcmp(command, "LIGHT:OFF") == 0)
   {
       manual_light_state = 0;
       if (!light_auto_mode)
       {
           LEDs_Off();
       }
       HAL_UART_Transmit(&huart1,
                         (uint8_t*)"ACK:LIGHT_OFF\n",
                         strlen("ACK:LIGHT_OFF\n"),
                         100);
   }
   else if (strcmp(command, "STATUS?") == 0)
   {
      Send_Status();
   }
   else
   {
       HAL_UART_Transmit(&huart1,
                         (uint8_t*)"ERR:UNKNOWN_COMMAND\n",
                         strlen("ERR:UNKNOWN_COMMAND\n"),
                         100);
   }
}

void Send_Status(void)
{
   char status_msg[120];
   int temp = (int)FanControl_GetTemperature();
   int hum  = (int)FanControl_GetHumidity();
   int fan  = FanControl_GetPWM();
   sprintf(status_msg,
           "MOTION=%d,TEMP=%d,HUM=%d,LIGHT=%s,FAN=%d,ALARM=%d\n",
           motion_detected,
           temp,
           hum,
           manual_light_state ? "ON" : "OFF",
           fan,
           alarm_active);
   HAL_UART_Transmit(&huart1,
                     (uint8_t*)status_msg,
                     strlen(status_msg),
                     100);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
