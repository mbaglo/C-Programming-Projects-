/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os2.h"
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RNG_HandleTypeDef hrng;

TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;

/* Definitions for RandomNum */
osThreadId_t RandomNumHandle;
const osThreadAttr_t RandomNum_attributes = {
  .name = "RandomNum",
  .stack_size = 500 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Bank */
osThreadId_t BankHandle;
const osThreadAttr_t Bank_attributes = {
  .name = "Bank",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Teller1 */
osThreadId_t Teller1Handle;
const osThreadAttr_t Teller1_attributes = {
  .name = "Teller1",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Teller2 */
osThreadId_t Teller2Handle;
const osThreadAttr_t Teller2_attributes = {
  .name = "Teller2",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Teller3 */
osThreadId_t Teller3Handle;
const osThreadAttr_t Teller3_attributes = {
  .name = "Teller3",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Metrics */
osThreadId_t MetricsHandle;
const osThreadAttr_t Metrics_attributes = {
  .name = "Metrics",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Break1 */
osThreadId_t Break1Handle;
const osThreadAttr_t Break1_attributes = {
  .name = "Break1",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Break2 */
osThreadId_t Break2Handle;
const osThreadAttr_t Break2_attributes = {
  .name = "Break2",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Break3 */
osThreadId_t Break3Handle;
const osThreadAttr_t Break3_attributes = {
  .name = "Break3",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for bank_queue */
osMessageQueueId_t bank_queueHandle;
uint8_t bank_queueBuffer[ 20 * sizeof( uint32_t ) ];
osStaticMessageQDef_t bank_queueControlBlock;
const osMessageQueueAttr_t bank_queue_attributes = {
  .name = "bank_queue",
  .cb_mem = &bank_queueControlBlock,
  .cb_size = sizeof(bank_queueControlBlock),
  .mq_mem = &bank_queueBuffer,
  .mq_size = sizeof(bank_queueBuffer)
};
/* Definitions for uart_mutex */
osMutexId_t uart_mutexHandle;
const osMutexAttr_t uart_mutex_attributes = {
  .name = "uart_mutex"
};
/* Definitions for teller_mutex */
osMutexId_t teller_mutexHandle;
const osMutexAttr_t teller_mutex_attributes = {
  .name = "teller_mutex"
};
/* Definitions for queue_mutex */
osMutexId_t queue_mutexHandle;
const osMutexAttr_t queue_mutex_attributes = {
  .name = "queue_mutex"
};
/* Definitions for uart_semaphore */
osSemaphoreId_t uart_semaphoreHandle;
const osSemaphoreAttr_t uart_semaphore_attributes = {
  .name = "uart_semaphore"
};
/* Definitions for break1_semaphore */
osSemaphoreId_t break1_semaphoreHandle;
const osSemaphoreAttr_t break1_semaphore_attributes = {
  .name = "break1_semaphore"
};
/* Definitions for break2_semaphore */
osSemaphoreId_t break2_semaphoreHandle;
const osSemaphoreAttr_t break2_semaphore_attributes = {
  .name = "break2_semaphore"
};
/* Definitions for break3_semaphore */
osSemaphoreId_t break3_semaphoreHandle;
const osSemaphoreAttr_t break3_semaphore_attributes = {
  .name = "break3_semaphore"
};
/* Definitions for queue_ready_semaphore */
osSemaphoreId_t queue_ready_semaphoreHandle;
osStaticSemaphoreDef_t queue_ready_semaphoreControlBlock;
const osSemaphoreAttr_t queue_ready_semaphore_attributes = {
  .name = "queue_ready_semaphore",
  .cb_mem = &queue_ready_semaphoreControlBlock,
  .cb_size = sizeof(queue_ready_semaphoreControlBlock),
};
/* Definitions for teller_available_semaphore */
osSemaphoreId_t teller_available_semaphoreHandle;
osStaticSemaphoreDef_t teller_available_semaphoreControlBlock;
const osSemaphoreAttr_t teller_available_semaphore_attributes = {
  .name = "teller_available_semaphore",
  .cb_mem = &teller_available_semaphoreControlBlock,
  .cb_size = sizeof(teller_available_semaphoreControlBlock),
};
/* USER CODE BEGIN PV */

/* Segment byte maps for numbers 0 to 9 */

const char SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
const char SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};


char buffer[1024];
uint32_t rnd;
uint32_t hours = 0;
uint32_t minutes = 0;
volatile uint32_t ul_idle_cycle_count = 0;  // Idle cycle counter
volatile uint8_t uart_tx_done = 0;
/* Simulation variables */
uint32_t simulated_time = 0;  // Simulated time
int bank_open = 1;            // Bank open status
uint32_t arrival_time;
queue_t queue_status = { 0, 0, 0, 0, 0};  // Queue statistics initialization
teller_t teller[NUM_TELLERS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Teller 1
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Teller 2
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // Teller 3
};



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RNG_Init(void);
static void MX_TIM5_Init(void);
void random_num_task(void *argument);
extern void bank_task(void *argument);
extern void teller_task(void *argument);
extern void metrics_task(void *argument);
extern void break_task(void *argument);

/* USER CODE BEGIN PFP */
void shiftOut(uint8_t val);
void WriteNumberToSegment(int Segment, int Value);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void uart_transmit(char *buffer);
void display_customers_in_queue_on_7seg(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void shiftOut(uint8_t val)
{
  for (int ii = 0x80; ii; ii >>= 1) {
    HAL_GPIO_WritePin(SHLD_D7_SEG7_Clock_GPIO_Port, SHLD_D7_SEG7_Clock_Pin, GPIO_PIN_RESET);
    if (ii & val)
      HAL_GPIO_WritePin(SHLD_D8_SEG7_Data_GPIO_Port, SHLD_D8_SEG7_Data_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(SHLD_D8_SEG7_Data_GPIO_Port, SHLD_D8_SEG7_Data_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(SHLD_D7_SEG7_Clock_GPIO_Port, SHLD_D7_SEG7_Clock_Pin, GPIO_PIN_SET);
  }
}

/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(int Segment, int Value)
{
  HAL_GPIO_WritePin(SHLD_D4_SEG7_Latch_GPIO_Port, SHLD_D4_SEG7_Latch_Pin, GPIO_PIN_RESET);
  shiftOut(SEGMENT_MAP[Value]);
  shiftOut(SEGMENT_SELECT[Segment]);
  HAL_GPIO_WritePin(SHLD_D4_SEG7_Latch_GPIO_Port, SHLD_D4_SEG7_Latch_Pin, GPIO_PIN_SET);
}
/**
 * Function to check if the bank is open
 */
int is_bank_open(uint32_t simulated_time) {
    return (simulated_time >= OPEN_TIME && simulated_time < CLOSE_TIME);
}
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
  MX_USART2_UART_Init();
  MX_RNG_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim5);
  //MX_FREERTOS_Init();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of uart_mutex */
  uart_mutexHandle = osMutexNew(&uart_mutex_attributes);

  /* creation of teller_mutex */
  teller_mutexHandle = osMutexNew(&teller_mutex_attributes);

  /* creation of queue_mutex */
  queue_mutexHandle = osMutexNew(&queue_mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */



  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of uart_semaphore */
  uart_semaphoreHandle = osSemaphoreNew(1, 0, &uart_semaphore_attributes);

  /* creation of break1_semaphore */
  break1_semaphoreHandle = osSemaphoreNew(1, 0, &break1_semaphore_attributes);

  /* creation of break2_semaphore */
  break2_semaphoreHandle = osSemaphoreNew(1, 0, &break2_semaphore_attributes);

  /* creation of break3_semaphore */
  break3_semaphoreHandle = osSemaphoreNew(1, 0, &break3_semaphore_attributes);

  /* creation of queue_ready_semaphore */
  queue_ready_semaphoreHandle = osSemaphoreNew(20, 20, &queue_ready_semaphore_attributes);

  /* creation of teller_available_semaphore */
  teller_available_semaphoreHandle = osSemaphoreNew(3, 3, &teller_available_semaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* creation of myBinarySem01 */

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of bank_queue */
  bank_queueHandle = osMessageQueueNew (20, sizeof(uint32_t), &bank_queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of RandomNum */
  RandomNumHandle = osThreadNew(random_num_task, NULL, &RandomNum_attributes);

  /* creation of Bank */
  BankHandle = osThreadNew(bank_task, NULL, &Bank_attributes);

  /* creation of Teller1 */
  Teller1Handle = osThreadNew(teller_task, (void*) &teller[0], &Teller1_attributes);

  /* creation of Teller2 */
  Teller2Handle = osThreadNew(teller_task, (void*) &teller[1], &Teller2_attributes);

  /* creation of Teller3 */
  Teller3Handle = osThreadNew(teller_task, (void*) &teller[2], &Teller3_attributes);

  /* creation of Metrics */
  MetricsHandle = osThreadNew(metrics_task, NULL, &Metrics_attributes);

  /* creation of Break1 */
  Break1Handle = osThreadNew(break_task, (void*) &teller[0], &Break1_attributes);

  /* creation of Break2 */
  Break2Handle = osThreadNew(break_task, (void*) &teller[1], &Break2_attributes);

  /* creation of Break3 */
  Break3Handle = osThreadNew(break_task, (void*) &teller[2], &Break3_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 7999;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 999;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

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
  HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

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
  HAL_GPIO_WritePin(SHLD_A4_GPIO_Port, SHLD_A4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SHLD_D13_Pin|SHLD_D12_Pin|SHLD_D11_Pin|SHLD_D7_SEG7_Clock_Pin
                          |SHLD_D8_SEG7_Data_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SHLD_D3_Pin|SHLD_D4_SEG7_Latch_Pin|SHLD_D10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SHLD_A5_Pin SHLD_D9_Pin */
  GPIO_InitStruct.Pin = SHLD_A5_Pin|SHLD_D9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SHLD_A4_Pin */
  GPIO_InitStruct.Pin = SHLD_A4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SHLD_A4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SHLD_A0_Pin SHLD_D2_Pin */
  GPIO_InitStruct.Pin = SHLD_A0_Pin|SHLD_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SHLD_A1_Pin SHLD_A2_Pin */
  GPIO_InitStruct.Pin = SHLD_A1_Pin|SHLD_A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SHLD_D13_Pin SHLD_D12_Pin SHLD_D11_Pin SHLD_D7_SEG7_Clock_Pin
                           SHLD_D8_SEG7_Data_Pin */
  GPIO_InitStruct.Pin = SHLD_D13_Pin|SHLD_D12_Pin|SHLD_D11_Pin|SHLD_D7_SEG7_Clock_Pin
                          |SHLD_D8_SEG7_Data_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SHLD_A3_Pin */
  GPIO_InitStruct.Pin = SHLD_A3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SHLD_A3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SHLD_D6_Pin SHLD_D5_Pin */
  GPIO_InitStruct.Pin = SHLD_D6_Pin|SHLD_D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SHLD_D3_Pin SHLD_D4_SEG7_Latch_Pin SHLD_D10_Pin */
  GPIO_InitStruct.Pin = SHLD_D3_Pin|SHLD_D4_SEG7_Latch_Pin|SHLD_D10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SHLD_D15_Pin SHLD_D14_Pin */
  GPIO_InitStruct.Pin = SHLD_D15_Pin|SHLD_D14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */

  // Enable EXTI interrupt for A1 (PA1), A2 (PA4), and A3 (PB0)
  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);  // Set priority for A1
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);  // Set priority for A2
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);  // Set priority for A3

  HAL_NVIC_EnableIRQ(EXTI1_IRQn);  // Enable EXTI1 for A1 (PA1)
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);  // Enable EXTI4 for A2 (PA4)
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);  // Enable EXTI0 for A3 (PB0)


/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief Period elapsed callback in non-blocking mode
  * @note  This function is called when TIM5 interrupt takes place.
  *        It makes a direct call to HAL_IncTick() to increment
  *        a global variable "uwTick" used as application time base.
  * @param htim : TIM handle
  * @retval None
  */


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    // Check if the correct UART interface
    if (huart == &huart2) {
        // Release the semaphore to signal transmission completion
        osSemaphoreRelease(uart_semaphoreHandle);
    }
}

void uart_transmit(char *buffer) {
    // Acquire UART mutex to ensure exclusive access
    osMutexAcquire(uart_mutexHandle, osWaitForever);

    // Transmit the message using non-blocking UART interrupt
    HAL_UART_Transmit_IT(&huart2, (uint8_t*)buffer, strlen(buffer));

    // Wait for the semaphore to be released once the transmission completes
    osSemaphoreAcquire(uart_semaphoreHandle, osWaitForever);

    // Release the mutex after transmission is complete
    osMutexRelease(uart_mutexHandle);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM5)
  {
    // Check if bank is still open before incrementing time
    if (simulated_time < CLOSE_TIME) {
        HAL_IncTick();  // Increment system tick
        simulated_time = HAL_GetTick();


    }
  }
}
void display_customers_in_queue_on_7seg(void) {
    static int current_display_digit = 0;  // 0 for ones, 1 for tens
    int queue_count = queue_status.current_customers;

    // Ensure the queue count does not exceed 99
    if (queue_count > 99) {
        queue_count = 99;
    }

    int tens = queue_count / 10;
    int ones = queue_count % 10;

    // Alternating between the ones and tens digits
    if (current_display_digit == 0) {
        WriteNumberToSegment(1, ones);  // Show ones digit on segment 1
        current_display_digit = 1;      // Switch to tens next time
    } else {
        WriteNumberToSegment(0, tens);  // Show tens digit on segment 0
        current_display_digit = 0;      // Switch to ones next time
    }
}
// Interrupt handler function for button presses
// Interrupt handler function for button presses
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    static uint32_t last_button_press_time_sim = 0;  // Track the simulated time for debouncing

    // Debounce mechanism: Ignore button presses if less than 1 simulated minute apart
    if ((simulated_time - last_button_press_time_sim) < 100) {  // 100ms in simulation = 1 simulated minute
        return;  // Ignore button presses that occur too quickly
    }
    last_button_press_time_sim = simulated_time;  // Update last press time with simulated time

    // Button for Teller 1 (PA1)
    if (GPIO_Pin == GPIO_PIN_1) {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) {
            // Button is pressed down (start forced break)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);  // Turn on LED D1 (PA5)
            teller[0].force_break = 1;  // Indicate forced break for Teller 1
            osSemaphoreRelease(break1_semaphoreHandle);  // Signal semaphore for Teller 1
        } else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET) {
            // Button is released (end forced break)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);  // Turn off LED D1 (PA5)
            teller[0].force_break = 0;  // Indicate forced break is over
        }
    }

    // Button for Teller 2 (PA4)
    if (GPIO_Pin == GPIO_PIN_4) {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) {
            // Button is pressed down (start forced break)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);  // Turn on LED D2 (PA6)
            teller[1].force_break = 1;  // Indicate forced break for Teller 2
            osSemaphoreRelease(break2_semaphoreHandle);  // Signal semaphore for Teller 2
        } else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET) {
            // Button is released (end forced break)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);  // Turn off LED D2 (PA6)
            teller[1].force_break = 0;  // Indicate forced break is over
        }
    }

    // Button for Teller 3 (PB0)
    if (GPIO_Pin == GPIO_PIN_0) {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) {
            // Button is pressed down (start forced break)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);  // Turn on LED D3 (PA7)
            teller[2].force_break = 1;  // Indicate forced break for Teller 3
            osSemaphoreRelease(break3_semaphoreHandle);  // Signal semaphore for Teller 3
        } else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET) {
            // Button is released (end forced break)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);  // Turn off LED D3 (PA7)
            teller[2].force_break = 0;  // Indicate forced break is over
        }
    }
}



/* USER CODE END 4 */

/* USER CODE BEGIN Header_random_num_task */
/**
  * @brief  Function implementing the RandomNum thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_random_num_task */
__weak void random_num_task(void *argument)
{
  /* USER CODE BEGIN 5 */
	 /* Infinite loop */
	  for(;;)
	  {
	   osDelay(1000);
	   HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
	   sprintf(buffer,"\r\nRandom Number: %u\r\n", (unsigned int)rnd);
	   uart_transmit(buffer);


	  }

  /* USER CODE END 5 */
}

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

#ifdef  USE_FULL_ASSERT
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
