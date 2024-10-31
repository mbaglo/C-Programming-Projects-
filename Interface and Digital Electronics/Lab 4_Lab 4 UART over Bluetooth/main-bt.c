#include "msp.h"
#include "uart.h"         // Main serial functionality
#include "led.h"          // Optional: Could be useful to display info/warnings
#include <stdio.h>        // General functionality
#include <string.h>       // Useful for string operations and memset

#define CHAR_COUNT 80     // Size of a console line

void LED1_On(void);
void LED1_Off(void);

int main()
{
    // Initialize LEDs (optional)
    led1_init();  // Initialize LED1 for status indication

    // Initialize UART0 and UART2
    uart0_init();  // Initialize UART0 (PC)
    uart2_init();  // Initialize UART2 (Phone)

    // Display startup message on both devices
    uart0_put("Chatroom has booted up\nPC> ");
    uart2_put("Chatroom has booted up\nPhone> ");

    // Declare and reset buffers and print prompts
    char pcBuffer[CHAR_COUNT + 1] = {0};      // Buffer for PC input
    char phoneBuffer[CHAR_COUNT + 1] = {0};   // Buffer for phone input
    int pcIndex = 0;                           // Current index in PC buffer
    int phoneIndex = 0;                        // Current index in phone buffer

    /* Control loop */
    while(1)
    {
        /* Check if there is data available from the PC */
        if (uart0_dataAvailable() == TRUE)
        {
            LED1_On();
            // Retrieve the character
            char ch = uart0_getchar(); // get the character

            /* If statements to handle the character */
            if (ch == '\n' || ch == '\r') {
                
						
							
			/* Newline: send PC buffer to Phone and clear PC buffer */
            /* PC Side*/
							
				pcBuffer[pcIndex] = '\0';              // Null-terminate string
                uart2_put("\r\nPC>");                 // Prompt for next input on Phone
                uart2_put(pcBuffer);                   // Send PC message to Phone
                pcIndex = 0;                           // Reset index
				uart2_put("\r\nPhone>");                //Prompt for next input on Phone incase PC sends message while Phone was typing
				uart2_put(phoneBuffer);                 // echo phone message to Phone
                memset(pcBuffer, 0, CHAR_COUNT);      // Clear buffer
                uart0_put("\r\nPC> ");                 // Prompt for next input on PC
								
            } else if (ch == '\b') {  								// Handle backspace
                if (pcIndex > 0) {
                    pcIndex--;                         // Move index back
                    pcBuffer[pcIndex] = '\0';         // Null-terminate to erase last character
					uart0_put("\b \b");                // Erase last character on terminal
                }
            } else {
                if (pcIndex < CHAR_COUNT) {
                    pcBuffer[pcIndex++] = ch;          // Add char to buffer and increment index
                    uart0_putchar(ch);                 // Echo back to PC terminal
                }
            }
            LED1_Off();
        }

        /* Check if there is data available from the Phone */
        if (uart2_dataAvailable() == TRUE)
        {
            LED1_On();
            // Retrieve the character
            char ch = uart2_getchar(); // get the character

            /* If statements to handle the character */
            if (ch == '\n' || ch == '\r') {
							
							
                /* Newline: send Phone buffer to PC and clear Phone buffer */
                /*Phone Side*/
							
			    phoneBuffer[phoneIndex] = '\0';        // Null-terminate string
                uart0_put("\r\nPhone> ");              // Prompt for next input on PC
                uart0_put(phoneBuffer);                // Send Phone message to PC
                phoneIndex = 0;                        // Reset index
				uart0_put("\r\nPC> ");                  //Prompt for next input on PC incase Phone sends message while PC was typing
				uart0_put(pcBuffer);                    // echo PC message to PC
                memset(phoneBuffer, 0, CHAR_COUNT);   // Clear buffer
                uart2_put("\r\nPhone> ");              // Prompt for next input on Phone
            } else if (ch == '\b') {                    // Handle backspace
                if (phoneIndex > 0) {
                    phoneIndex--;                      // Move index back
                    phoneBuffer[phoneIndex] = '\0';    // Null-terminate to erase last character
					uart2_put("\b \b");                // Erase last character on terminal
                }
            } else {
                if (phoneIndex < CHAR_COUNT) {
                    phoneBuffer[phoneIndex++] = ch;    // Add char to buffer and increment index
                    uart2_putchar(ch);                 // Echo back to Phone terminal
                }
            }
            LED1_Off();
        }
    }
}

void LED1_On(void) {
    P1->OUT |= BIT0;  // Assuming LED1 is connected to P1.0
}

void LED1_Off(void) {
    P1->OUT &= ~BIT0;  // Assuming LED1 is connected to P1.0
}
