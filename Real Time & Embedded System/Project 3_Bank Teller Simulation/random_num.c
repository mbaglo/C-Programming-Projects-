/*
 * random_num.c
 *
 *  Created on: Oct 20, 2024
 *      Author: gm6629
 */

#include "random_num.h"
#include <stdio.h>
#include <string.h>
#include "main.h"

/* Declare the external variables */
extern RNG_HandleTypeDef hrng;
extern osThreadId_t RandomNumHandle;
extern const osThreadAttr_t RandomNum_attributes;

extern char buffer[1024];
extern uint32_t rnd;

/* Function to initialize the random number task */
void random_num_init(void) {
    // Create the random number task thread
    RandomNumHandle = osThreadNew(random_num_task, NULL, &RandomNum_attributes);
}

/* Random number task implementation */
void random_num_task(void *argument) {
    /* Infinite loop */
    for(;;) {
        osDelay(1000);  // Delay for 1 second
        HAL_RNG_GenerateRandomNumber(&hrng, &rnd);  // Generate a random number
        //snprintf(buffer, sizeof(buffer), "\r\nRandom Number: %u\r\n", (unsigned int)rnd);
       // uart_transmit(buffer);  // Transmit the random number via UART
    }
}

