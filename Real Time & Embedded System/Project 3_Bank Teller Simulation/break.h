/*
 * break.h
 *
 *  Created on: Oct 22, 2024
 *      Author: gm6629
 */

#ifndef INC_BREAK_H_
#define INC_BREAK_H_

#include "cmsis_os.h"
#include "teller.h"



void break_init(void);
void break_task(void *argument);
void handle_breaks(teller_t *teller);
int break_start(teller_t *teller);



#endif /* INC_BREAK_H_ */
