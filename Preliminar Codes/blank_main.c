/* USER CODE BEGIN Header */
/**
  **************************
  * @file           : main.c
  * @brief          : Main program body
  **************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
 
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
 
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
 
/* USER CODE BEGIN PV */
 
/* USER CODE END PV */
 
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
 
 
/* USER CODE BEGIN PFP */
 
/* USER CODE END PFP */
 
 
 
  /* USER CODE BEGIN PFP */
 
  /* USER CODE END PFP */
 
 
  /* Private user code ---------------------------------------------------------*/
  /* USER CODE BEGIN 0 */
  void processSequence(void);
  // void processB(void);
 
  // --------From the old code--------
  // Flag
  /* volatile int b_pressed_flag = 0;
 
  void EXTI2_IRQHandler(void){
    NVIC_ClearPendingIRQ(EXTI2_IRQn);
    b_pressed_flag = 1;
    EXTI->PR |= (0x01 << 2); // Clear the EXTI pending register
  }
 
  void EXTI15_10_IRQHandler(void){
    if(EXTI->PR & (0x01 << 13)){
      NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
      b_pressed_flag = 2;
      EXTI->PR |= (0x01 << 13); // Clear the EXTI pending register
    }
  } */
 
  /* USER CODE END 0 */
 
 
/**
  * @brief  The application entry point.
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
 
  // Abilitazine clock per GPIOA e GPIOC
   RCC->AHB1ENR |= (0x1 << 0);
   RCC->AHB1ENR |= (0x1 << 2);
 
   /* Configure PC0 as an output */
   // GPIOC0 <-> PC_0 (LED1)
   GPIOC->MODER &= ~(0x03 << 0);   // Clear bits [1:0] → PC0 mode
   GPIOC->MODER |= (0x01 << 0);   // Set as OUTPUT (01)
   //GPIOC->OTYPER &= ~(0x1 << 0);   // Bit 0 → Push-pull
   GPIOC->PUPDR &= ~(0x03 << 0);   // Bits [1:0] → No pull-up/pull-down
   GPIOC->ODR   &= ~(0x1 << 0);    // Bit 0 → LED OFF initially
 
   /* PC1 as an output */
   // GPIOC1 <-> PC_1 (LED2)
   GPIOC->MODER &= ~(0x03 << 2);   // Clear bits [3:2] → PC1 mode
   GPIOC->MODER |=  (0x01 << 2);   // Set as OUTPUT (01)
   //GPIOC->OTYPER &= ~(0x1 << 1);   // Bit 1 → Push-pull
   GPIOC->PUPDR &= ~(0x03 << 2);   // Bits [3:2] → No pull-up/pull-down
   GPIOC->ODR   &= ~(0x1 << 1);    // Bit 1 → LED OFF initially
 
   /* PC2 as an output */
   // GPIOC2 <-> PC_2 (LED2)
   GPIOC->MODER &= ~(0x03 << 4);   // Clear bits [5:4] → PC2 mode
   GPIOC->MODER |=  (0x01 << 4);   // Set as OUTPUT (01)
   // GPIOC->OTYPER &= ~(0x1 << 2);   // Bit 2 → Push-pull
   GPIOC->PUPDR &= ~(0x03 << 4);   // Bits [5:4] → No pull-up/pull-down
   GPIOC->ODR   &= ~(0x1 << 2);    // Bit 2 → LED OFF initially
   
   /*  PC3 as an input (GPIOC Pin 3) */
   // GPIOC3 <-> PC_3 (BUTTON)
   GPIOC->MODER &= ~(0x03 << 6);   // Clear bits [7:6] → PC3 mode
   GPIOC->MODER |=  (0x00 << 6);   // Set as INPUT (00)
   GPIOC->PUPDR &= ~(0x03 << 6);   // Clear bits [7:6]
   GPIOC->PUPDR |=  (0x01 << 6);   // Enable Pull-Up
 
   processSequence();
 
   /* OLD INPUTS AND OUTPUTS CONFIGURATION
   GPIOC->MODER &= ~(0x03 << 26); // Clear MODER GPIOC13
   GPIOC->MODER |= (0x00 << 26);  // Set Input
   GPIOC->PUPDR &= ~(0x03 << 26); // Clear PUPDR GPIOC13
   GPIOC->PUPDR |= (0x01 << 26);  // Set Pull-Up
 
   //GPIOC2 <-> PC_2
   GPIOC->MODER &= ~(0x03 << 4); // Clear MODER GPIOC2
   GPIOC->MODER |= (0x00 << 4);  // Set Input
   GPIOC->PUPDR &= ~(0x03 << 4); // Clear PUPDR GPIOC2
   GPIOC->PUPDR |= (0x01 << 4);  // Set Pull-Up
 
    LED su GPIOA5 e GPIOA10
   // GPIOA5 <-> PA_5 (LD2)
   GPIOA->MODER &= ~(0x03 << 10); // Clear MODER GPIOA5
   GPIOA->MODER |= (0x01 << 10);  // Set Output
   GPIOA->OTYPER &= ~(0x1 << 5);  // Output Push-Pull
   GPIOA->PUPDR &= ~(0x03 << 10); // Clear PUPDR GPIOA5
   GPIOA->PUPDR |= (0x01 << 10);  // Set Pull-Up
   GPIOA->ODR |= (0x1 << 5);      // Set GPIOA5 Output
   */
 
   
   /* Gestione interrupts
   // Abilitazione clock per SYSCFG
   RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
 
   SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PC;  // External interrupt on GPIOC2
   SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;   //0x0020U; // External interrupt on GPIOC13
 
   EXTI->IMR |= (0x01 << 13) | (0x01 << 2);     // Set not masked interrupt
   EXTI->RTSR |= (0x01 << 13) | (0x01 << 2);    // Rising Edge
   //EXTI->PR |= (0x01 << 13) | (0x01 << 2); // Clear the EXTI pending register
 
   // Abilitazione Interrupt
   __asm volatile ("cpsie i" : : : "memory"); // Change Processor State, Enable Interrupts
 
   // Gestione NVIC
 
   // PC_13
   NVIC_SetPriority(EXTI15_10_IRQn, 0);
   NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
   NVIC_EnableIRQ(EXTI15_10_IRQn);
   // PC_2
   NVIC_SetPriority(EXTI2_IRQn, 0);
   NVIC_ClearPendingIRQ(EXTI2_IRQn);
   NVIC_EnableIRQ(EXTI2_IRQn);
   */
 
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
 
  /* USER CODE END 2 */
 
  /* Infinite loop */
  while (1){
 
  }
}
 
void processSequence(void)
{
/*
  * ODR register mapping:
  * Bit 0 → PC0
  * Bit 1 → PC1
  * Bit 2 → PC2
  */
  /* TURN ON */
  GPIOC->ODR |= (0x1 << 0);   // Set bit 0 → PC0 ON
  HAL_Delay(1000);
  GPIOC->ODR |= (0x1 << 1);   // Set bit 1 → PC1 ON
  HAL_Delay(1000);
  GPIOC->ODR |= (0x1 << 2);   // Set bit 2 → PC2 ON
  HAL_Delay(1000);
  /* TURN OFF (REVERSE) */
  GPIOC->ODR &= ~(0x1 << 2);  // Clear bit 2 → PC2 OFF
  HAL_Delay(1000);
  GPIOC->ODR &= ~(0x1 << 1);  // Clear bit 1 → PC1 OFF
  HAL_Delay(1000);
  GPIOC->ODR &= ~(0x1 << 0);  // Clear bit 0 → PC0 OFF
  HAL_Delay(1000);
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
 
/* USER CODE END 4 */
 
/**
  * @brief  This function is executed in case of error occurrence.
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
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
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
