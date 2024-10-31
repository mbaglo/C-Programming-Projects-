/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "semphr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "stm32l4xx_hal_rng.h"    // For RNG functions
#include "stm32l4xx_hal_uart.h"   // For UART functions
#include "cmsis_os.h"             // CMSIS-RTOS API
#include "stm32l4xx_hal.h"        // STM32 HAL definitions
#include "task.h"                 // FreeRTOS task handling


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define NUM_TELLERS 3
// Struct Definitions
// Structure to represent the queue statistics

typedef struct {

    uint32_t max_depth;         // Maximum depth of the queue during the day
    uint32_t customers_served;  // Total number of customers served during the day
    uint32_t max_wait_time;     // Maximum time a customer waited in the queue
    uint32_t total_wait_time;   // Total time all customers spent waiting in the queue
    uint32_t current_customers; // Current number of customers in the queue
} queue_t;

// Structure to represent a bank teller's statistics
typedef struct {
    int id;                     // Teller ID (unique identifier for the teller)
    uint32_t customers_served;   // Number of customers served by the teller
    uint32_t total_service_time; // Total time the teller spent serving customers
    uint32_t max_service_time;   // Maximum time spent on a single customer transaction
    uint32_t total_idle_time;    // Total time the teller spent idle (waiting for customers)
    uint32_t max_idle_time;      // Maximum idle time for the teller between customers
    uint32_t is_busy;            // 1 if the teller is currently busy, 0 if idle
    uint32_t next_break_time;    // Simulated time for the next scheduled break
    uint32_t break_duration;     // Duration of the teller's break
    uint32_t num_breaks;         // Number of breaks the teller has taken during the day
    uint32_t total_break_time;   // Total time spent on breaks by the teller
    uint32_t longest_break_time; // Longest break taken by the teller
    uint32_t shortest_break_time;// Shortest break taken by the teller
    uint32_t force_break;         // 1 if a break is forced via button press, 0 otherwise
    uint32_t idle_start_time;    // Simulated time when the teller started being idle
    uint32_t on_break;           // 1 if the teller is currently on a break, 0 if not

} teller_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// Global Variables
extern uint32_t arrival_time;  // Correct type declaration
extern char buffer[1024];
extern RNG_HandleTypeDef hrng;
extern UART_HandleTypeDef huart2;                // UART handler for communication
extern volatile uint32_t ul_idle_cycle_count;    // Idle cycle counter for system idle tracking
extern uint32_t simulated_time;                  // Simulated time representing bank operations
extern int bank_open;                            // Flag to indicate if the bank is open
extern queue_t queue_status;                     // Structure holding the queue statistics
extern teller_t teller[NUM_TELLERS];             // Array holding statistics for each teller
extern osMessageQueueId_t bank_queue;        // Message queue for customer task
extern osMessageQueueId_t bank_queueHandle;
extern const osMessageQueueAttr_t bank_queue_attributes;
extern osMutexId_t uart_mutexHandle;
extern const osMutexAttr_t uart_mutex_attributes;
extern osMutexId_t teller_mutexHandle;
extern const osMutexAttr_t teller_mutex_attributes;
extern osMutexId_t queue_mutexHandle;
extern const osMutexAttr_t queue_mutex_attributes;
extern osSemaphoreId_t uart_semaphoreHandle;
extern const osSemaphoreAttr_t uart_semaphore_attributes;
extern osSemaphoreId_t queue_ready_semaphoreHandle;
extern const osSemaphoreAttr_t queue_ready_semaphore_attributes;
extern osSemaphoreId_t teller_available_semaphoreHandle;
extern const osSemaphoreAttr_t teller_available_semaphore_attributes;
extern osSemaphoreId_t break1_semaphoreHandle;
extern const osSemaphoreAttr_t break1_semaphore_attributes;
extern osSemaphoreId_t break2_semaphoreHandle;
extern const osSemaphoreAttr_t break2_semaphore_attributes;
extern osSemaphoreId_t break3_semaphoreHandle;
extern const osSemaphoreAttr_t break3_semaphore_attributes;
extern osThreadId_t RandomNumHandle;
extern const osThreadAttr_t RandomNum_attributes;
extern osThreadId_t BankHandle;
extern const osThreadAttr_t Bank_attributes;
extern osThreadId_t Teller1Handle;
extern const osThreadAttr_t Teller1_attributes;
extern osThreadId_t Teller2Handle;
extern const osThreadAttr_t Teller2_attributes;
extern osThreadId_t Teller3Handle;
extern const osThreadAttr_t Teller3_attributes;
extern osThreadId_t MetricsHandle;
extern const osThreadAttr_t Metrics_attributes;
extern osThreadId_t Break1Handle;
extern const osThreadAttr_t Break1_attributes;
extern osThreadId_t Break2Handle;
extern const osThreadAttr_t Break2_attributes;
extern osThreadId_t Break3Handle;
extern const osThreadAttr_t Break3_attributes;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
// Time constants in milliseconds (real-time milliseconds)
#define MIN_ARRIVAL_TIME (100)   // 1 minute in real-time milliseconds (100ms)
#define MAX_ARRIVAL_TIME (400)   // 4 minutes in real-time milliseconds (400ms)
#define MIN_TRANSACTION_TIME (50) // 30 seconds in simulated milliseconds (50ms)
#define MAX_TRANSACTION_TIME (800)   // 8 minutes in simulated milliseconds (800ms)
#define OPEN_TIME (0)                   // 0ms - start time
#define CLOSE_TIME (42000)              // 42,000ms - close time (7 hours later)
#define MIN_BREAK_INTERVAL (3000) // Minimum time between breaks (30 minutes)
#define MAX_BREAK_INTERVAL (6000) // Maximum time between breaks (60 minutes)
#define MIN_BREAK_DURATION (100)  // Minimum break duration (1 minute)
#define MAX_BREAK_DURATION (400)  // Maximum break duration (4 minutes)

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
int is_bank_open(uint32_t simulated_time);
extern void bank_task(void *argument);
extern void teller_task(void *argument);
extern void metrics_task(void *argument);
extern void break_task(void *argument);
void vApplicationIdleHook(void);
void MX_FREERTOS_Init(void);
void WriteNumberToSegment(int segment, int value);
void random_num_task(void *argument);
void uart_transmit(char *buffer);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define SHLD_A5_Pin GPIO_PIN_0
#define SHLD_A5_GPIO_Port GPIOC
#define SHLD_A4_Pin GPIO_PIN_1
#define SHLD_A4_GPIO_Port GPIOC
#define SHLD_A0_Pin GPIO_PIN_0
#define SHLD_A0_GPIO_Port GPIOA
#define SHLD_A1_Pin GPIO_PIN_1
#define SHLD_A1_GPIO_Port GPIOA
#define SHLD_A1_EXTI_IRQn EXTI1_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define SHLD_A2_Pin GPIO_PIN_4
#define SHLD_A2_GPIO_Port GPIOA
#define SHLD_A2_EXTI_IRQn EXTI4_IRQn
#define SHLD_D13_Pin GPIO_PIN_5
#define SHLD_D13_GPIO_Port GPIOA
#define SHLD_D12_Pin GPIO_PIN_6
#define SHLD_D12_GPIO_Port GPIOA
#define SHLD_D11_Pin GPIO_PIN_7
#define SHLD_D11_GPIO_Port GPIOA
#define SHLD_A3_Pin GPIO_PIN_0
#define SHLD_A3_GPIO_Port GPIOB
#define SHLD_A3_EXTI_IRQn EXTI0_IRQn
#define SHLD_D6_Pin GPIO_PIN_10
#define SHLD_D6_GPIO_Port GPIOB
#define SHLD_D9_Pin GPIO_PIN_7
#define SHLD_D9_GPIO_Port GPIOC
#define SHLD_D7_SEG7_Clock_Pin GPIO_PIN_8
#define SHLD_D7_SEG7_Clock_GPIO_Port GPIOA
#define SHLD_D8_SEG7_Data_Pin GPIO_PIN_9
#define SHLD_D8_SEG7_Data_GPIO_Port GPIOA
#define SHLD_D2_Pin GPIO_PIN_10
#define SHLD_D2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SHLD_D3_Pin GPIO_PIN_3
#define SHLD_D3_GPIO_Port GPIOB
#define SHLD_D5_Pin GPIO_PIN_4
#define SHLD_D5_GPIO_Port GPIOB
#define SHLD_D4_SEG7_Latch_Pin GPIO_PIN_5
#define SHLD_D4_SEG7_Latch_GPIO_Port GPIOB
#define SHLD_D10_Pin GPIO_PIN_6
#define SHLD_D10_GPIO_Port GPIOB
#define SHLD_D15_Pin GPIO_PIN_8
#define SHLD_D15_GPIO_Port GPIOB
#define SHLD_D14_Pin GPIO_PIN_9
#define SHLD_D14_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
