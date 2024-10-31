/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bank.h" // Include your modular header files
#include "teller.h"
#include "metrics.h"
#include "random_num.h"
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
/* USER CODE BEGIN Variables */


/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */


	ul_idle_cycle_count++;  // Increment idle cycle counter for tracking idle time
}
/* USER CODE END 2 */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  // Initialize any FreeRTOS-specific features (like timers, etc.)
  /* USER CODE END Init */

  /* Create the mutex(es) */
  // Mutex initialization (uart, queue, teller mutexes)
  uart_mutexHandle = osMutexNew(&uart_mutex_attributes);
  queue_mutexHandle = osMutexNew(&queue_mutex_attributes);
  teller_mutexHandle = osMutexNew(&teller_mutex_attributes);

  /* Create the semaphore(s) */
  // Semaphore initialization (queue, teller, uart semaphores)
  uart_semaphoreHandle = osSemaphoreNew(1, 0, &uart_semaphore_attributes);
  queue_ready_semaphoreHandle = osSemaphoreNew(20, 0, &queue_ready_semaphore_attributes);
  teller_available_semaphoreHandle = osSemaphoreNew(3, 0, &teller_available_semaphore_attributes);
  break1_semaphoreHandle = osSemaphoreNew(1, 0, &break1_semaphore_attributes);
  break2_semaphoreHandle = osSemaphoreNew(1, 0, &break2_semaphore_attributes);
  break3_semaphoreHandle = osSemaphoreNew(1, 0, &break3_semaphore_attributes);


  /* Create the queue(s) */
  bank_queueHandle = osMessageQueueNew(20, sizeof(uint32_t), &bank_queue_attributes);

  /* Create the thread(s) */
  // Instead of creating tasks here, call modular init functions to initialize each task
  bank_init();   // Initializes bank task
  teller_init();     // Initializes teller tasks (Teller1, Teller2, Teller3)
  metrics_init();    // Initializes metrics task
  random_num_init(); //Initializes random number tasks
 // break_init();
  }
/* USER CODE END Application */

