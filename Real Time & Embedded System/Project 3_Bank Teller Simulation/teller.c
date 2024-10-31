/*
 * teller.c
 *
 *  Created on: Oct 20, 2024
 *      Author: gm6629
 */

#include "teller.h"
#include "cmsis_os.h"
#include "main.h"
#include "metrics.h"
#include "bank.h"

// External variables
extern char buffer[1024];
extern uint32_t rnd;
extern uint32_t simulated_time;  // Simulated time for bank operations
extern osSemaphoreId_t queue_ready_semaphoreHandle;
extern osSemaphoreId_t teller_available_semaphoreHandle;  // Semaphore to signal teller availability
extern osMessageQueueId_t bank_queueHandle;
extern osMutexId_t teller_mutexHandle;
extern osMutexId_t queue_mutexHandle;
extern queue_t queue_status;
extern uint32_t arrival_time;
extern uint32_t hours, minutes;

// Function to initialize teller tasks and semaphores
void teller_init(void) {
    // Create teller tasks for each teller
    Teller1Handle = osThreadNew(teller_task, (void*) &teller[0], &Teller1_attributes);
    Teller2Handle = osThreadNew(teller_task, (void*) &teller[1], &Teller2_attributes);
    Teller3Handle = osThreadNew(teller_task, (void*) &teller[2], &Teller3_attributes);

    // Create binary semaphores for each teller to handle break events
    break1_semaphoreHandle = osSemaphoreNew(1, 0, &break1_semaphore_attributes);
    break2_semaphoreHandle = osSemaphoreNew(1, 0, &break2_semaphore_attributes);
    break3_semaphoreHandle = osSemaphoreNew(1, 0, &break3_semaphore_attributes);

    // Initialize the teller's next break time, break duration, and idle start time
     for (int i = 0; i < NUM_TELLERS; i++) {
     HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
     teller[i].next_break_time = simulated_time + ((rnd % (MAX_BREAK_INTERVAL - MIN_BREAK_INTERVAL + 1)) + MIN_BREAK_INTERVAL);
     teller[i].break_duration = (rnd % (MAX_BREAK_DURATION - MIN_BREAK_DURATION + 1)) + MIN_BREAK_DURATION;
     teller[i].idle_start_time = simulated_time;
     }

    // Create necessary semaphores for available tellers
    teller_available_semaphoreHandle = osSemaphoreNew(NUM_TELLERS, NUM_TELLERS, &teller_available_semaphore_attributes);  // Allow all tellers to be available initially
}

// Teller task function: Manages customer servicing and break handling
void teller_task(void *argument) {
    teller_t *teller = (teller_t *)argument;  // Cast argument to teller structure
    rnd = 0;

    // Infinite loop to wait for customers and serve them
    while (1) {
        // Teller specific button or break handling logic
        if (teller->on_break == 0) {

            // Wait for a customer to be available in the queue by acquiring the queue_ready_semaphore
            osSemaphoreAcquire(queue_ready_semaphoreHandle, osWaitForever);  // Wait until a customer is ready

            // Check if there are customers in the queue
            if (osMessageQueueGet(bank_queueHandle, &arrival_time, NULL, osWaitForever) == osOK) {
                // Before serving the customer, acquire the teller_available semaphore
                osSemaphoreAcquire(teller_available_semaphoreHandle, osWaitForever);  // Ensure a teller is available

                // Process customer transaction
                HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
                uint32_t transaction_time = (rnd % (MAX_TRANSACTION_TIME - MIN_TRANSACTION_TIME + 1)) + MIN_TRANSACTION_TIME;

                osMutexAcquire(queue_mutexHandle, osWaitForever);

                // Calculate and print the customer wait time
                uint32_t customer_wait_time = simulated_time - arrival_time;  // Wait time in milliseconds

                // Update queue statistics
                queue_status.total_wait_time += customer_wait_time;
                if (customer_wait_time > queue_status.max_wait_time) {
                    queue_status.max_wait_time = customer_wait_time;  // Update maximum wait time
                }
                osMutexRelease(queue_mutexHandle);

                // Before serving the customer, calculate idle time
                osMutexAcquire(teller_mutexHandle, osWaitForever);
                uint32_t idle_time = simulated_time - teller->idle_start_time;  // Calculate idle time since last idle start
                teller->total_idle_time += idle_time;  // Add to total idle time

                if (idle_time > teller->max_idle_time) {
                    teller->max_idle_time = idle_time;  // Update max idle time if this was the longest idle period
                }
                osMutexRelease(teller_mutexHandle);

                // Mark teller as busy before serving customer
                osMutexAcquire(teller_mutexHandle, osWaitForever);
                teller->is_busy = 1;  // Mark teller as busy
                osMutexRelease(teller_mutexHandle);

                serve_customer(teller, transaction_time);  // Call function to serve the customer

            +    // After serving a customer, start idle time tracking again
                osMutexAcquire(teller_mutexHandle, osWaitForever);
                teller->is_busy = 0;  // Mark teller as idle
                teller->idle_start_time = simulated_time;  // Reset the idle start time for the next idle period
                osMutexRelease(teller_mutexHandle);

                // Release teller semaphore (teller is now available)
                osSemaphoreRelease(teller_available_semaphoreHandle);
            }

            // Check if the teller has come back from a forced break
            if (!teller->force_break && teller->on_break) {
                teller->on_break = 0;  // Reset the on_break flag when the teller resumes work
            }
        } else {
            // Teller is on break, so skip customer processing
            osDelay(100);  // Prevent tight looping
        }
    }
}


// Function to serve the customer and update statistics
void serve_customer(teller_t *teller, uint32_t transaction_time) {
    uint32_t service_start_time = simulated_time;  // Record when service starts

    // Simulate the time it takes to serve the customer
    osDelay(transaction_time);

    // Update teller statistics
    osMutexAcquire(teller_mutexHandle, osWaitForever);
    teller->customers_served++;
    uint32_t service_time = simulated_time - service_start_time;  // Calculate service time
    teller->total_service_time += service_time;
    if (service_time > teller->max_service_time) {
        teller->max_service_time = service_time;
    }
    osMutexRelease(teller_mutexHandle);

    // Update queue statistics
    osMutexAcquire(queue_mutexHandle, osWaitForever);
    queue_status.customers_served++;
    queue_status.current_customers--;
    osMutexRelease(queue_mutexHandle);

    // Log service completion
    convert_simulated_time_to_clock(simulated_time, &hours, &minutes);
   // snprintf(buffer, sizeof(buffer), "Teller %d finished serving a customer at time %02lu:%02lu (Clock Time)\r\n", teller->id, hours, minutes);
   // uart_transmit(buffer);
}

