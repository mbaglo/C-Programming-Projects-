/*
 * File:        uart.c
 * Purpose:     Provide UART routines for serial IO
 *
 * Notes:		
 *
 */

#include "msp.h"
#include "uart.h"  // you need to create this file with the function prototypes
#include "Common.h"  // from Lab1 code

#define BAUD_RATE 9600      //default baud rate 
extern uint32_t SystemCoreClock;  // clock rate of MCU


void uart0_init()
{
	// Set the UART to RESET state
	EUSCI_A0->CTLW0 |= BIT0;//Set the UART to RESET state (set bit0 of EUSCI_A0->CTLW0 register to '1'
	
	// Clear all relevant bits in CTLW0 register
	EUSCI_A0->CTLW0 &= ~BITF; // bit15=0, no parity bits
	EUSCI_A0->CTLW0 &= ~BITD;	// bit13=0, LSB first
	EUSCI_A0->CTLW0 &= ~BITC;	// bit12=0, 8-bit data length
	EUSCI_A0->CTLW0 &= ~BITB;	// bit11=0, 1 stop bit
	EUSCI_A0->CTLW0 &= ~(BITA|BIT9|BIT8);	// bits10-8=000, asynchronous UART mode
	EUSCI_A0->CTLW0 &= ~BIT5;	// bit5=0, reject erroneous characters and do not set flag
	EUSCI_A0->CTLW0 &= ~BIT4;	// bit4=0, do not set flag for break characters
	EUSCI_A0->CTLW0 &= ~BIT3;	// bit3=0, not dormant
	EUSCI_A0->CTLW0 &= ~BIT2;	// bit2=0, transmit data, not address (not used here)
	EUSCI_A0->CTLW0 &= ~BIT1;	// bit1=0, do not transmit break (not used here)
														

	// set CTLW0 - hold logic and configure clock source to SMCLK
	EUSCI_A0->CTLW0 |= BIT0;// Keep in reset
	EUSCI_A0->CTLW0 |= (BIT7|BIT6);	// bits7-6=11,   clock source to SMCLK
	
	
	// Baud Rate configuration 
	uint16_t brw = SystemCoreClock / BAUD_RATE;	// N = clock/baud rate = clock_speed/BAUD_RATE
	EUSCI_A0->BRW = brw;// set BRW register
	

	 // clear first and second modulation stage bit fields
	EUSCI_A0->MCTLW = 0; // clear MCTLW register;  
	
	
// Configure UART pins for RX and TX
	P1SEL0 = BIT3;// P1.3 = TxD
	P1SEL0 = BIT2;// P1.2 = RxD
	// we will be using P1.2, P1.3 for RX and TX but not in I/O mode, so we set Port1 SEL1=0 and SEL0=1
	P1->SEL0 |= (BIT2 | BIT3);// set BIT2 & BIT3 of P1SEL0 - UART0
	P1->SEL1 &=~ (BIT2 | BIT3);// clear BIT2 & BIT3 of P1SEL0 - UART0
	


	// CTLW0 register - release from reset state
	EUSCI_A0->CTLW0 &=~ BIT0;// Take UART out of reset

	// disable interrupts (transmit ready, start received, transmit empty, receive full)	     
	EUSCI_A0->IE = 0;// clear IE register; 

}

BYTE uart0_getchar()
{
	BYTE inChar;
	
	// Wait for data
	while(!(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)); // IFG register

	// read character and store in inChar variable
	inChar = EUSCI_A0->RXBUF ;// RXBUF register
	
	//Return the 8-bit data from the receiver 
	return(inChar);

}

void uart0_putchar(char ch)
{
	// Wait until transmission of previous bit is complete 
	
	while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)) // IFG register
	{};
	// send ch character to uart
	// TXBUF register 
	 EUSCI_A0->TXBUF = ch;
	

}

void uart0_put(char *ptr_str)
{
	while(*ptr_str != 0)
		uart0_putchar(*ptr_str++);
}

