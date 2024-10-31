#include "msp.h"
#include "led.h"
#include "Common.h" 

// Initialize LED 1 connected to P1.0
void led1_init(void)
{
	// configure PortPin for LED1 as port I/O 
	P1->SEL1 &= ~BIT0; // select Port1 Bit0 for use as port I/O
	P1->SEL0 &= ~BIT0; // select Port1 Bit0 for use as port I/O
	
	// make built-in LED1 LED high drive strength
	P1->DS|=BIT0; // set high drive strength

	// make built-in LED1 out	 
	P1->DIR|=BIT0; // set BIT 0 as output

	// turn off LED	
	P1->OUT &=~BIT0; // set P1.0 to '0'	
}

// Initialize RGB LED connected to P2.0 (Red), P2.1 (Green), and P2.2 (Blue)
void led2_init(void)
{
	// configure PortPin for LED2 as port I/O 
 P2-> SEL1 &= ~BIT0; // select Port2 Bit0 for use as port I/O
 P2-> SEL0 &= ~BIT0; // select Port2 Bit0 for use as port I/O
 P2-> SEL1 &= ~BIT1; // select Port2 Bit1 for use as port I/O
 P2-> SEL0 &= ~BIT1; // select Port2 Bit1 for use as port I/O
 P2-> SEL1 &= ~BIT2; // select Port2 Bit2 for use as port I/O
 P2-> SEL0 &= ~BIT2; // select Port2 Bit2 for use as port I/O


	// make built-in LED2 LEDs high drive strength

	P2->DS |=BIT0; // set BIT O to high drive strength
	P2->DS |=BIT1; // set BIT 1 to high drive strength
	P2->DS |=BIT2; // set BIT 2 to high drive strength
	
	// make built-in LED2 out	 
	P2->DIR |=BIT0; // set BIT 0 as output
	P2->DIR |=BIT1; // set BIT 1 as output
	P2->DIR |=BIT2; // set BIT 2 as output
	

	// turn off LED
	P2->OUT &=~BIT0; // set P2.0 to '0'
	P2->OUT &=~BIT1; // set P2.1 to '0'
	P2->OUT &=~BIT2; // set P2.2 to '0'
	
	
}

