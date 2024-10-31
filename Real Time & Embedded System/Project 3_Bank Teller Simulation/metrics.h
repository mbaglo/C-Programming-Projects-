/*
 * metrics.h
 *
 *  Created on: Oct 20, 2024
 *      Author: gm6629
 */

#ifndef INC_METRICS_H_
#define INC_METRICS_H_

#include <stdint.h>
#include "cmsis_os.h"
#include "main.h"
#include "stdio.h"
#include <stdbool.h>
// Function prototypes
void metrics_init(void);
extern void metrics_task(void *argument);

// Helper functions for time conversion and metrics
void convert_simulated_time_to_clock(uint32_t simulated_time, uint32_t *hours, uint32_t *minutes);
void display_metrics(void);
void display_customers_in_queue_on_7seg(void);
void generate_end_of_day_report(void);
bool bank_closed_and_empty();
void convert_simulated_time_to_clock(uint32_t simulated_time, uint32_t *hours, uint32_t *minutes);
#endif /* INC_METRICS_H_ */
