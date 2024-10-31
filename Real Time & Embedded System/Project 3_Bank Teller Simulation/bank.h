/*
 * bank.h
 *
 *  Created on: Oct 20, 2024
 *      Author: gm6629
 */

#ifndef INC_BANK_H_
#define INC_BANK_H_
#include "main.h"

// Function prototypes for bank initialization and task
void bank_init(void);                   // Initializes the bank task and message queue
extern void bank_task(void *argument);  // Bank task function to generate customers and handle customer queue
void update_queue_status(void);         // Function to update queue statistics
void log_customer_creation(uint32_t arrival_time);  // Function to log customer creation details
void log_bank_closed(void);             // Function to log when the bank is closed
uint32_t random_delay(void);            // Function to generate random delay for customer arrival

#endif /* INC_BANK_H_ */
