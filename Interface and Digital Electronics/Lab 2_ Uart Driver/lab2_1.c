/*
Title: UART Driver
Description:Writing a UART driver for the MSP432
Authors: Chinyere Gloria Mbaka
Date: January 24 2024


All rights reserved.
*/

#include<stdio.h>
#include "msp.h"
#include "uart.h"			// you will need to create this file
#define BAUD_RATE 9600      //default baud rate 
#define CHAR_COUNT 10      //change this to modify the max. permissible length of a sentence

void put (char *put_str);

int main()
{
	
	uart0_init(); // clock speed
	
	
	put("\r\n IDE Lab2: UART's are awesome!");    /*Transmit this through UART*/

		
}  

void put(char *ptr_str)
{
	while(*ptr_str)
		uart0_putchar(*ptr_str++);
}
