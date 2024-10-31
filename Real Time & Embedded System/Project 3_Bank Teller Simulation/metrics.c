/*
 * metrics_task.c
 *
 *  Created on: Oct 21, 2024
 *      Author: gm6629
 */

#include "metrics.h"
#include "cmsis_os.h"
#include "main.h"
#include <stdio.h>
#include <stdbool.h>

extern char buffer[1024];  // Buffer for UART transmission
extern volatile uint32_t ul_idle_cycle_count;  // System-wide idle count
extern osMutexId_t teller_mutexHandle;
extern osMutexId_t queue_mutexHandle;

extern queue_t queue_status;
extern teller_t teller[NUM_TELLERS];
extern uint32_t simulated_time;  // Simulated time for bank operations

void metrics_init(void) {
    // Create metrics task
    MetricsHandle = osThreadNew(metrics_task, NULL, &Metrics_attributes);
}

/**
 * @brief Convert simulated time in milliseconds to HH:MM clock format (from 9:00 AM).
 * @param simulated_time Time in milliseconds (e.g., 6000ms = 1 minute).
 * @param hours Pointer to store the hour value.
 * @param minutes Pointer to store the minute value.
 */
void convert_simulated_time_to_clock(uint32_t simulated_time, uint32_t *hours, uint32_t *minutes) {
    uint32_t total_minutes = simulated_time / 100;  // Simulated minutes (each 100 ms is 1 minute)
    *hours = 9 + (total_minutes / 60);  // Add to 9 AM
    *minutes = total_minutes % 60;
}

/**
 * @brief Display the current metrics for tellers and queue status.
 */
void display_metrics(void) {
    uint32_t hours, minutes;

    // Convert simulated time to clock time
    convert_simulated_time_to_clock(simulated_time, &hours, &minutes);

    // 1. Display simulated time as HH:MM (clock format)
    snprintf(buffer, sizeof(buffer), "\r\nSimulated Time: %02lu:%02lu (Clock Time)\r\n", hours, minutes);
    uart_transmit(buffer);

    // 2. Display the number of customers waiting in the queue
    osMutexAcquire(queue_mutexHandle, osWaitForever);  // Protect queue status
    snprintf(buffer, sizeof(buffer), "Customers in Queue: %lu\r\n", queue_status.current_customers);
    uart_transmit(buffer);
    osMutexRelease(queue_mutexHandle);

    // 3. Display the status of each teller and the number of customers served
    osMutexAcquire(teller_mutexHandle, osWaitForever);  // Protect teller statistics
    for (int i = 0; i < NUM_TELLERS; i++) {
        const char *status = teller[i].is_busy ? "Busy" : (teller[i].on_break ? "On Break" : "Idle");
        snprintf(buffer, sizeof(buffer), "Teller %d Status: %s, Customers Served: %lu\r\n",
                 teller[i].id, status, teller[i].customers_served);
        uart_transmit(buffer);
    }
    osMutexRelease(teller_mutexHandle);
}

/**
 * @brief Display the number of customers in the queue on the 7-segment display.
 */


/**
 * @brief Generate the end-of-day report when the bank is closed and the queue is empty.
 */
void generate_end_of_day_report(void) {
	osDelay(10000);
    // Final end-of-day report
    snprintf(buffer, sizeof(buffer), "\r\n--- End of Day Report ---\r\n");
    uart_transmit(buffer);

    // Total customers served
    snprintf(buffer, sizeof(buffer), "Total Customers Served: %lu\r\n", queue_status.customers_served);
    uart_transmit(buffer);

    // Customers served by each teller
    for (int i = 0; i < NUM_TELLERS; i++) {
        snprintf(buffer, sizeof(buffer), "Teller %d served %lu customers\r\n", teller[i].id, teller[i].customers_served);
        uart_transmit(buffer);
    }

    // Average wait time for customers
    if (queue_status.customers_served > 0) {
        uint32_t avg_wait_time = queue_status.total_wait_time / queue_status.customers_served;
        snprintf(buffer, sizeof(buffer), "Average Wait Time: %lu mimutes\r\n", avg_wait_time /100);
        uart_transmit(buffer);
    }

    // Maximum wait time
    snprintf(buffer, sizeof(buffer), "Maximum Customer Wait Time: %lu minutes\r\n", queue_status.max_wait_time / 100);  // 100 ms = 1 minute
    uart_transmit(buffer);
    osDelay(100);


    // Teller statistics
    for (int i = 0; i < NUM_TELLERS; i++) {
        if (teller[i].customers_served > 0) {
            uint32_t avg_service_time = teller[i].total_service_time / teller[i].customers_served;
            snprintf(buffer, sizeof(buffer), "Teller %d average service time: %lu minutes\r\n", teller[i].id, avg_service_time / 100);
            uart_transmit(buffer);
            osDelay(100);
        }


        uint32_t avg_idle_time = teller[i].total_idle_time /NUM_TELLERS;
        snprintf(buffer, sizeof(buffer), "Teller %d average idle time: %lu minutes\r\n", teller[i].id, avg_idle_time / 100);
        uart_transmit(buffer);
        osDelay(100);

        snprintf(buffer, sizeof(buffer), "Teller %d max transaction time: %lu minutes\r\n", teller[i].id, teller[i].max_service_time / 100);
        uart_transmit(buffer);
        osDelay(100);

        // Number of breaks for each teller
        snprintf(buffer, sizeof(buffer), "Teller %d total number of breaks: %lu\r\n", teller[i].id, teller[i].num_breaks);
        uart_transmit(buffer);
        osDelay(100);

        // Average break time for each teller
        if (teller[i].num_breaks > 0) {
            uint32_t avg_break_time = teller[i].total_break_time / teller[i].num_breaks;
            snprintf(buffer, sizeof(buffer), "Teller %d average break time: %lu minutes\r\n", teller[i].id, avg_break_time / 100);
        } else {
            snprintf(buffer, sizeof(buffer), "Teller %d average break time: No breaks taken\r\n", teller[i].id);
        }
        uart_transmit(buffer);
        osDelay(100);

        // Longest break time for each teller
        snprintf(buffer, sizeof(buffer), "Teller %d longest break time: %lu minutes\r\n", teller[i].id, teller[i].longest_break_time / 100);
        uart_transmit(buffer);
        osDelay(100);

        // Shortest break time for each teller (if available)
        if (teller[i].num_breaks > 0) {
            snprintf(buffer, sizeof(buffer), "Teller %d shortest break time: %lu minutes\r\n", teller[i].id, teller[i].shortest_break_time / 100);
        } else {
            snprintf(buffer, sizeof(buffer), "Teller %d shortest break time: No breaks taken\r\n", teller[i].id);
        }
        uart_transmit(buffer);
    }
    osDelay(100);
    // Queue depth
    snprintf(buffer, sizeof(buffer), "Max Queue Depth: %lu\r\n", queue_status.max_depth);
    uart_transmit(buffer);
    osDelay(100);

    // Idle hook count: Display it both in milliseconds and converted to simulation minutes
    snprintf(buffer, sizeof(buffer), "Idle Hook Count: %lu\r\n", ul_idle_cycle_count);
    uart_transmit(buffer);
    osDelay(100);

    // Convert idle hook count to real-time milliseconds (1000ms per second / tick rate)
    uint32_t idle_time_ms = ul_idle_cycle_count * (1000 / configTICK_RATE_HZ);

    // Adjust to simulation time where 100ms in real-time equals 1 minute in simulation
    uint32_t idle_simulation_minutes = (idle_time_ms / 100) * 1;  // Convert 100ms to 1 simulated minute

    // Print idle hook count in real-time milliseconds
    snprintf(buffer, sizeof(buffer), "System Idle Time (Real Time): %lu ms\r\n", idle_time_ms);
    uart_transmit(buffer);
    osDelay(100);

    // Print idle hook count in simulation minutes
    snprintf(buffer, sizeof(buffer), "System Idle Time (Simulation Time): %lu minutes\r\n", idle_simulation_minutes);
    uart_transmit(buffer);
    osDelay(100);

}

/**
 * @brief Check if the bank is closed and the queue is empty.
 * @return true if the bank is closed and the queue is empty, otherwise false.
 */
bool bank_closed_and_empty(){
    // Check if the simulated time is past the bank's closing time
    if (simulated_time >= CLOSE_TIME && queue_status.current_customers == 0) {
        return true;
    }
    return false;
}

/**
 * @brief Task for monitoring and reporting metrics.
 */
void metrics_task(void *argument) {
    uint8_t report_generated = 0;  // Flag to ensure the report is generated only once

    while (1) {
        display_customers_in_queue_on_7seg();
        osDelay(3000);  // Update metrics every 3s

        // Display real-time metrics
        display_metrics();

        // Generate the end-of-day report if the bank is closed and no customers are left in the queue
        if (bank_closed_and_empty() && !report_generated) {
            osDelay(2000);  // Wait a little before generating the report

            generate_end_of_day_report();
            report_generated = 1;  // Set the flag to indicate the report has been generated

            // Exit the task once the report is generated
            osThreadExit();
        }
    }
}

