/*
Title: UART Driver
Description: UART driver for the MSP432
Authors: Chinyere Gloria Mbaka
Date: February 07, 2024

All rights reserved.
*/

#include<stdio.h>
#include "msp.h"
#include "uart.h"         
#include "Common.h"
#include "led.h"

#define BAUD_RATE 9600    // Default baud rate
 


void SetLEDColorAndPrint(char code);

int main(void) {
    led2_init();  // Initialize RGB LED
    uart2_init();  // Initialize UART2 for Bluetooth communication

    char ch; //Received Message

    while (1) {
        if (uart2_dataAvailable()) {  // Check if data is available on UART2
            ch = uart2_getchar();  // Read a character from UART2
            SetLEDColorAndPrint(ch);  // Set LED color and print the color name
        }
    }
}

void SetLEDColorAndPrint(char ch) {
    switch (ch) {
        case '0':  // OFF
            P2->OUT &= ~(BIT0 | BIT1 | BIT2);  // Turn off all LEDs
            uart2_put("OFF\r\n");
            break;
        case '1':  // RED
            P2->OUT = (P2->OUT & ~(BIT1 | BIT2)) | BIT0;  // Turn on Red LED only
            uart2_put("Red\r\n");
            break;
        case '2':  // BLUE
            P2->OUT = (P2->OUT & ~(BIT0 | BIT1)) | BIT2;  // Turn on Blue LED only
            uart2_put("Blue\r\n");
            break;
        case '3':  // GREEN
            P2->OUT = (P2->OUT & ~(BIT0 | BIT2)) | BIT1;  // Turn on Green LED only
            uart2_put("Green\r\n");
            break;
        default:  // Invalid Input
            uart2_put("Invalid Input\r\n");
            break;
    }
}
