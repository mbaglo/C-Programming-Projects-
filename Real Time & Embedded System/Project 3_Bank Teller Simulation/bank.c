/*
 * bank.c
 *
 *  Created on: Oct 20, 2024
 *      Author: gm6629
 */

#include "bank.h"
#include "cmsis_os.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

// Define bank-related variables
extern osMessageQueueId_t bank_queueHandle;
extern const osMessageQueueAttr_t bank_queue_attributes;
extern osSemaphoreId_t queue_ready_semaphoreHandle;
extern uint32_t simulated_time;  // Simulated time
extern uint32_t rnd;  // Random number generator
extern queue_t queue_status;  // Global queue statistics
extern char buffer[1024];  // UART buffer for logging
extern uint32_t arrival_time;  // Use correct variable

// Maximum number of customers in the queue
#define QUEUE_CAPACITY 20

// Function to initialize the bank task and message queue
void bank_init(void) {
    // Create the bank task
    BankHandle = osThreadNew(bank_task, NULL, &Bank_attributes);

    // Create the message queue for customers, with a capacity of QUEUE_CAPACITY
    bank_queueHandle = osMessageQueueNew(QUEUE_CAPACITY, sizeof(uint32_t), &bank_queue_attributes);
}

// The main bank task function
void bank_task(void *argument) {
    // Infinite loop for generating customers
    while (1) {
        if (is_bank_open(simulated_time)) {
            rnd = 0;

            arrival_time = simulated_time;  // Set customer arrival time

            // Check if the queue is full before adding a new customer
            if (queue_status.current_customers < QUEUE_CAPACITY) {
                // Add customer to the queue
                if (osMessageQueuePut(bank_queueHandle, &arrival_time, 0, osWaitForever) == osOK) {
                    update_queue_status();  // Update the queue statistics

                    // Signal that a customer is ready to be served
                    osSemaphoreRelease(queue_ready_semaphoreHandle);

                    // Log the customer creation
                    log_customer_creation(arrival_time);

                    // Delay before generating the next customer (simulates time between customer arrivals)
                    osDelay(random_delay());
                }
            } else {
                // Log that the queue is full and customer couldn't be added
                log_queue_full();

                // Add a slight delay to avoid constantly checking the queue
                osDelay(500);  // Adjust the delay as needed
            }
        } else {
            // Log that the bank is closed and exit the thread
            log_bank_closed();
            osThreadExit();
        }
    }
}

// Function to update the queue statistics
void update_queue_status(void) {
    osMutexAcquire(queue_mutexHandle, osWaitForever);
    queue_status.current_customers++;
    if (queue_status.current_customers > queue_status.max_depth) {
        queue_status.max_depth = queue_status.current_customers;
    }
    osMutexRelease(queue_mutexHandle);
}

// Function to log customer creation
void log_customer_creation(uint32_t arrival_time) {
    char buffer[100];
   // snprintf(buffer, sizeof(buffer), "\r\nCustomer created with arrival time: %lu ms \r\n", arrival_time);
   // uart_transmit(buffer);  // Transmit the log via UART
}

// Function to log when the bank is closed
void log_bank_closed(void) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "\r\nBank is closed. No more customers will be processed.\r\n");
    uart_transmit(buffer);  // Transmit the log via UART
}

// Function to log when the queue is full
void log_queue_full(void) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "\r\nQueue is full. No new customers will be added.\r\n");
   uart_transmit(buffer);  // Transmit the log via UART
}

// Function to generate a random delay for customer arrival
uint32_t random_delay(void) {
    HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
    return (rnd % (MAX_ARRIVAL_TIME - MIN_ARRIVAL_TIME + 1)) + MIN_ARRIVAL_TIME;
}
