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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEFAULT_LOWER_LIMIT 950  // Default lower limit in microseconds
#define DEFAULT_UPPER_LIMIT 1050 // Default upper limit in microseconds (lower_limit + 100)
#define TOTAL_COUNT 1000         // Total number of measurements expected
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t IC_Val1 = 0;          // First capture value
uint32_t IC_Val2 = 0;          // Second capture value
uint32_t Difference = 0;       // Difference between captures
int Is_First_Captured = 0;     // Flag to indicate first capture
int Is_Second_Captured = 0;     // Flag to indicate second capture
uint32_t lower_limit = DEFAULT_LOWER_LIMIT;   // Lower limit for pulse width
uint32_t upper_limit = DEFAULT_UPPER_LIMIT;   // Upper limit for pulse width
uint32_t bucket[101] = {0};    // Array to store counts for pulse widths
char buffer[100];
volatile uint32_t capture_done = 0;  // Flag to track completion of measurements
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
bool power_on_self_test(void);
int set_timer_base(void);
void get_line(char *buffer, int max_length);
void print_results(void);
void uart2_transmit(const char *str);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void process_signal(void);
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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  // Perform Power-On Self Test (POST)
    while (!power_on_self_test())
    {
      // Loop until POST passes
    }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Display the current limits
	          snprintf(buffer, sizeof(buffer), "\r\nCurrent lower limit is %lu, upper limit is %lu.\r\n", lower_limit, upper_limit);
	          uart2_transmit(buffer);

	          // Prompt user for new lower limit or keep the default
	          uart2_transmit("Enter lower limit (50-9950) or press Enter to keep default: ");
	          get_line(buffer, sizeof(buffer));  // Get user input

	          // Set timer base and limits based on user input
	          while (set_timer_base() == 0);

	          // Reset state for a new set of measurements
	          capture_done = 0;
	          Is_First_Captured = 0;
	          memset(bucket, 0, sizeof(bucket));

	          // Clear any pending timer flags
	          __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC1);

	          // Enable input capture interrupt and start capturing signals
	          HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);

	          // Main capture loop (1000 measurements)
	          while (capture_done < 1000) {
	              if (Is_First_Captured == 1 && Is_Second_Captured == 1) {
	                  // Process the captured signal
	                  process_signal();

	                  // Reset flags for the next capture
	                  Is_First_Captured = 0;
	                  Is_Second_Captured = 0;


	              }
	          }
	          // Stop the timer after completing the measurements
	                  HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);

	                  // Print results to the terminal
	                  print_results();



    /* USER CODE END WHILE */
	                  return 0;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 79;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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


/* Power-On Self-Test Function */
/* Power-On Self-Test Function */
bool power_on_self_test(void) {
    uart2_transmit("\r\nPerforming POST...\r\n");

    // Clear the Timer Counter (CNT) and Capture/Compare Register (CCR1)
    __HAL_TIM_SET_COUNTER(&htim2, 0);  // Reset the timer counter
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC1);  // Clear the capture flag for channel 1

    // Start input capture on TIM2
    HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);

    // Wait for a capture event or timeout (100ms)
    uint32_t timeout = HAL_GetTick() + 100;
    while (!__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC1) && (HAL_GetTick() < timeout));

    // Check if a signal was detected
    if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC1)) {
        uart2_transmit("\r\nPOST Succeeded: Signal detected.\r\n");
        HAL_TIM_IC_Stop(&htim2, TIM_CHANNEL_1);
        return true;
    } else {
        uart2_transmit("\r\nPOST Failed: No signal detected. Try again? (y/n): ");
        char response[2] = {0};  // Initialize response buffer to store user input
        get_line(response, sizeof(response)); // Get user input

        // Check user input for 'y' or 'n'
        if (response[0] == 'y' || response[0] == 'Y') {
            return false;  // User wants to try POST again
        } else if (response[0] == 'n' || response[0] == 'N') {
            return true;  // User wants to stop, so treat it as POST passed to exit
        } else {
            uart2_transmit("\r\nInvalid input. Please enter 'y' or 'n'.\r\n");
            return false;  // Repeat POST again for invalid input
        }
    }
}


/* Sets timer base and user-configurable limits */
int set_timer_base(void) {


    // Check if the user input is empty (i.e., user pressed Enter)
    if (buffer[0] == '\0') {
        // User pressed Enter, keep current limits
        return 1;
    }

    // Convert the input to an integer value
    uint32_t new_lower_limit = atoi(buffer);

    // Validate the new lower limit
    if (new_lower_limit < 50 || new_lower_limit > 9950) {
        uart2_transmit("\r\nInvalid lower limit. Please enter a value between 50 and 9950.\r\n");
        return 0;
    }

    // Set new lower and upper limits
    lower_limit = new_lower_limit;
    upper_limit = lower_limit + 100;

    // Print the new limits
    snprintf(buffer, sizeof(buffer), "\r\nNew limits set: lower limit = %lu, upper limit = %lu\r\n", lower_limit, upper_limit);
    uart2_transmit(buffer);

    return 1;
}


/* Handles input capture interrupts */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        // Clear the interrupt flag
        __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);

        if (Is_First_Captured == 0) {
            // Capture the first rising edge
            IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            Is_First_Captured = 1;  // Set flag indicating the first capture
        } else if (Is_First_Captured == 1) {
            // Capture the second rising edge
            IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            Is_Second_Captured = 1;  // Set flag indicating the second capture


        }
    }
}


void process_signal(void){
	  // Calculate the difference in capture values

	                Difference = IC_Val2 - IC_Val1;

	            //snprintf(buffer, sizeof(buffer), "IC_Val1: %lu, IC_Val2: %lu, Difference: %lu us\r\n", IC_Val1, IC_Val2, Difference);
	           // uart2_transmit(buffer);


	            // Check if the difference is within the specified limits
	            if (Difference >= lower_limit && Difference <= upper_limit) {
	                uint32_t index = (Difference - lower_limit);
	                if (index < 101) {
	                    bucket[index]++;
		                  // Increment the count of completed measurements
		                  capture_done++;

	                    // Debug: Print updated bucket count
	                 //   snprintf(buffer, sizeof(buffer), "\r\nBucket[%lu] = %lu\r\n", index, bucket[index]);
	                 //   uart2_transmit(buffer);
	                }
	            }


}

/* Prints the results using UART2 */
void print_results(void) {
    uart2_transmit("\r\n        Time (us)     Count\r\n");  // Header for the results
    for (int i = 0; i < 101; i++) {
        if (bucket[i] > 0) {  // Only print buckets with counts greater than 0
            // Correctly format the output with newline
            snprintf(buffer, sizeof(buffer), "\r\nBucket[%d]: %lu     %lu\r\n",i, lower_limit + i, bucket[i]);
            uart2_transmit(buffer);
        }
    }
}


/* UART2 Transmit Function */
void uart2_transmit(const char *str) {
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

/* Captures a line of user input */
void get_line(char *buffer, int max_length) {
    int index = 0;
    while (index < max_length - 1) {
        char c;
        HAL_UART_Receive(&huart2, (uint8_t*)&c, 1, HAL_MAX_DELAY); // Read a character from UART

        // Echo the character back to the user
        HAL_UART_Transmit(&huart2, (uint8_t*)&c, 1, HAL_MAX_DELAY);

        if (c == '\r' || c == '\n') {
            buffer[index] = '\0'; // Null-terminate the string
            break;
        } else {
            buffer[index++] = c;
        }
    }
}

int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    ITM_SendChar(*ptr++);
  }
  return len;
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
