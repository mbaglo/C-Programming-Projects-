/*
 * breaktask.c
 *
 *  Created on: Oct 22, 2024
 *      Author: gm6629
 */

#include "break.h"
#include "teller.h"
#include "cmsis_os.h"
#include "main.h"
#include "metrics.h"
#include "bank.h"

// External variables
extern uint32_t simulated_time;
extern uint32_t rnd;
extern osMutexId_t teller_mutexHandle;
extern uint32_t hours, minutes;
extern char buffer[1024];

// Button flags to handle breaks
volatile uint8_t teller_break_flags[NUM_TELLERS] = {0};  // 0 for Teller 1, 1 for Teller 2, etc.

// Initialize break tasks
void break_init(void) {
    // Create the break tasks for each teller
    Break1Handle = osThreadNew(break_task, (void*) &teller[0], &Break1_attributes);
    Break2Handle = osThreadNew(break_task, (void*) &teller[1], &Break2_attributes);
    Break3Handle = osThreadNew(break_task, (void*) &teller[2], &Break3_attributes);


    // Initialize the teller's next break time and idle start time
       HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
       teller->next_break_time = simulated_time + ((rnd % (MAX_BREAK_INTERVAL - MIN_BREAK_INTERVAL + 1)) + MIN_BREAK_INTERVAL);
       teller->break_duration = (rnd % (MAX_BREAK_DURATION - MIN_BREAK_DURATION + 1)) + MIN_BREAK_DURATION;
       teller->idle_start_time = simulated_time;
}


void break_task(void *argument) {
    teller_t *teller = (teller_t *)argument;  // Cast argument to teller structure

    // Determine which semaphore to use for the given teller based on their ID
    osSemaphoreId_t break_semaphore = NULL;
    if (teller->id == 0) {
        break_semaphore = break1_semaphoreHandle;
    } else if (teller->id == 1) {
        break_semaphore = break2_semaphoreHandle;
    } else if (teller->id == 2) {
        break_semaphore = break3_semaphoreHandle;
    }

    while (1) {
        // Check if it's time for a scheduled break (and no forced break is active)
        if (!teller->force_break && simulated_time >= teller->next_break_time && !teller->on_break) {
            break_start(teller);  // Start the scheduled break
        }

        // Wait for semaphore signal indicating a button press (forced break)
        if (osSemaphoreAcquire(break_semaphore, 0) == osOK) {
            teller->force_break = 1;  // Set force break flag
            break_start(teller);  // Start the forced break
        }

        // Ensure the task doesn't run in a tight loop
        osDelay(100);  // Poll every 100ms to simulate break handling
    }
}

int break_start(teller_t *teller) {
    uint32_t break_start_time = simulated_time;  // Record break start time
    uint32_t break_time = 0;
    int break_completed = 0;
    uint32_t hours, minutes;

    // Mark teller as on break
    teller->on_break = 1;
    teller->is_busy = 0;  // Ensure the teller is marked as not busy

    // If this is a scheduled break, log accordingly
    if (!teller->force_break) {
    	convert_simulated_time_to_clock(simulated_time, &hours, &minutes);
        snprintf(buffer, sizeof(buffer), "Teller %d: Scheduled break started at %02lu:%02lu\r\n", teller->id, hours, minutes);
    } else {
        // If it's a forced break, log accordingly
    	convert_simulated_time_to_clock(simulated_time, &hours, &minutes);
        snprintf(buffer, sizeof(buffer), "Teller %d: Forced break started at %02lu:%02lu\r\n", teller->id, hours, minutes);
    }
    uart_transmit(buffer);

    // Break loop: this loop runs until the break is completed
    while (teller->on_break) {
        osDelay(100);  // Simulate time passing during the break

        // End break if forced break is over or the scheduled break duration has passed
        if (!teller->force_break && (simulated_time - break_start_time >= teller->break_duration)) {
            break_completed = 1;
            teller->on_break = 0;
            break;  // End the break
        }

        // If forced break is reactivated, continue the break
        if (teller->force_break) {
            continue;  // Teller remains on break while force_break is active
        }
    }

    // Calculate the break time
    break_time = simulated_time - break_start_time;

    // Update teller break statistics
    osMutexAcquire(teller_mutexHandle, osWaitForever);
    teller->num_breaks++;
    teller->total_break_time += break_time;

    // Update longest and shortest break times
    if (break_time > teller->longest_break_time) {
        teller->longest_break_time = break_time;
    }
    if (break_time < teller->shortest_break_time || teller->shortest_break_time == 0) {
        teller->shortest_break_time = break_time;
    }
    osMutexRelease(teller_mutexHandle);

    // Schedule next break time
    HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
    teller->next_break_time = simulated_time + ((rnd % (MAX_BREAK_INTERVAL - MIN_BREAK_INTERVAL + 1)) + MIN_BREAK_INTERVAL);
    teller->break_duration = (rnd % (MAX_BREAK_DURATION - MIN_BREAK_DURATION + 1)) + MIN_BREAK_DURATION;

    // Log the break completion
    convert_simulated_time_to_clock(simulated_time, &hours, &minutes);
    snprintf(buffer, sizeof(buffer), "Teller %d: Break finished at %02lu:%02lu\r\n", teller->id, hours, minutes);
    uart_transmit(buffer);

    // Clear the on_break flag and mark the break as completed
    teller->on_break = 0;

    return break_completed;
}
