#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "Common.h"  
#include "msp.h"

// Function prototypes for UART0 and UART2
void uart0_init(void);
void uart2_init(void);
void uart0_putchar(char ch);
void uart2_putchar(char ch);
BYTE uart0_getchar(void);
BYTE uart2_getchar(void);
void uart0_put(char *ptr_str);
void uart2_put(char *ptr_str);
BOOLEAN uart0_dataAvailable(void);
BOOLEAN uart2_dataAvailable(void);

#endif // UART_H
