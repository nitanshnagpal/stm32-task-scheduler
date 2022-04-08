/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Non-Preamptive Task Scheduler for STM32 (Without using RTOS)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
	
	* <h2><center>&copy; Copyright (c) 2021 TEMNEM LABS.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <cstring>
#include "lwmem.h"  /* Using Lightweight dynamic memory manager (By - Tilen MAJERLE) for dynamic memory allocation */
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
TIM_HandleTypeDef htim16;
UART_HandleTypeDef huart2;

char buffer[50] = {0}; /* Buffer to print messages on serial console */
uint32_t timer_val;
uint8_t scheduling_flag = 0; /* Flag to determine when to schedule for next task, 0 : Scheduler ready to schedule next task, 1 : Not ready for next task */
uint8_t timer_flag = 0;
uint32_t totalTaks = 0; /* It tells how many numbers of tasks we have currently */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM16_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */


void Task1(void);
void Task2(void);
void Task3(void);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char uart_buf[50];
  int uart_buf_len;
	struct task *ptr1, *ptr2, *ptr3; /* Task structure pointers */
  
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
  MX_TIM16_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	
	/* Initialize LwMEM */
 
    if (!lwmem_assignmem(regions, sizeof(regions) / sizeof(regions[0]))) {
        while (1) {}
    }
		
		ptr1 = (struct task*)lwmem_malloc(sizeof(struct task));                   /* Allocating memory for Task Structure ptr1 */
    if (ptr1 == NULL) {
        uint8_t len = sprintf(buffer, "Cannot allocate memory ptr1\r\n");
				HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, 100);
    }
		
		ptr1->TaskFunc = Task1;
		head_task_node = ptr1;
		totalTaks++;
		
		
		ptr2 = (struct task*)lwmem_malloc(sizeof(struct task));                   /* Allocating memory for Task Structure ptr2 */
    if (ptr2 == NULL) {
        uint8_t len = sprintf(buffer, "Cannot allocate memory ptr2\r\n");
				HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, 100);
    }
		ptr2->TaskFunc = Task2;
		
		add_task(ptr2); /* Add task 2 to task list */
		
		ptr3 = (struct task*)lwmem_malloc(sizeof(struct task));                   /* Allocating memory for Task Structure ptr3 */
    if (ptr3 == NULL) {
        uint8_t len = sprintf(buffer, "Cannot allocate memory ptr3\r\n");
				HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, 100);
    }
		ptr3->TaskFunc = Task3;
		
		add_task(ptr3); /* Add task 3 to task list */
		
		del_task(1); 		/* Delete Task 1 from task list */
	
	uart_buf_len = sprintf(uart_buf, "Task Scheduling Tutorial\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
	
	/* Start timer */
  HAL_TIM_Base_Start_IT(&htim16);
	timer_val = __HAL_TIM_GET_COUNTER(&htim16);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
		uint8_t i = 0;
		struct task *task_fun = head_task_node;
  while (1)
  {
    /* USER CODE END WHILE */
	
    /* USER CODE BEGIN 3 */
		
		
		if(scheduling_flag == 0){
			task_fun = head_task_node;
			for(i=0;i<(timer_flag - 1);i++)
			{
				task_fun = task_fun->next;
			}
			scheduling_flag = 1;
		}
		
		task_fun->TaskFunc(); /* Excecute Task function */
		
  }
  /* USER CODE END 3 */
}



void Task1() {
	sprintf(buffer, "Hello from task 1");
	HAL_UART_Transmit(&huart2, (uint8_t *)buffer, 17, 100);
	HAL_Delay(50);
}

void Task2() {
	sprintf(buffer, "Hello from task 2");
	HAL_UART_Transmit(&huart2, (uint8_t *)buffer, 17, 100);
	HAL_Delay(50);
}

void Task3() {
	sprintf(buffer, "Hello from task 3");
	HAL_UART_Transmit(&huart2, (uint8_t *)buffer, 17, 100);
	HAL_Delay(50);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM16)
	{
		scheduling_flag = 0;
		if(timer_flag == 0)
		{
			timer_flag = 1;
		}else if(timer_flag < totalTaks)
		{
			timer_flag = timer_flag + 1;
		}else if(timer_flag == totalTaks)
		{
			timer_flag = 1;
		}
	}
}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 7999;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 200;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
