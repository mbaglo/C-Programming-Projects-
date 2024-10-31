/*
 * teller.h
 *
 *  Created on: Oct 20, 2024
 *      Author: gm6629
 */

#ifndef INC_TELLER_H_
#define INC_TELLER_H_

#include "cmsis_os.h"
#include "main.h"
// Function prototypes

/**
 * @brief Initializes the teller tasks and semaphores.
 */
void teller_init(void);

/**
 * @brief The task function for a bank teller. Manages customer service and breaks.
 * @param argument Pointer to the teller structure.
 */
extern void teller_task(void *argument);

/**
 * @brief Function to serve a customer and update the teller's statistics.
 * @param teller Pointer to the teller structure.
 * @param customer The customer being served.
 */
void serve_customer(teller_t *teller, uint32_t transaction_time);  // Correct signature



#endif /* INC_TELLER_H_ */
