/*
 * random_num.h
 *
 *  Created on: Oct 20, 2024
 *      Author: gm6629
 */

#ifndef INC_RANDOM_NUM_H_
#define INC_RANDOM_NUM_H_

#include "cmsis_os.h"
#include "main.h"

/* Function prototypes */
void random_num_init(void);
extern void random_num_task(void *argument);

#endif /* INC_RANDOM_NUM_H_ */
