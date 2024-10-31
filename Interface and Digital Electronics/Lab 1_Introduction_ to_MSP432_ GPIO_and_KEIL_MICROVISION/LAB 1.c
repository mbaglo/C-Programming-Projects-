/* 
Title:  Introduction to MSP432 GPIO and KEIL MICROVISION
Purpose:  The purpose of this lab is to introduce the MSP432 microcontroller and GPIO control using KEIL MICROVISION.
Name:   CHINYERE GLORIA MBAKA
Date: 01/17/2024

This file prepared by LJBeato
01/11/2021
  
*/
#include "msp.h" 

#include "Common.h"
#define LED1 



// Main Program
// 1. Init LEDs
// 2. Init Switch
// 3. init state=0
// 4. If SW1 pressed, light LED1
// 5. if SW2 pressed, cycle next color triplet of RGB,CMY,W
//	First press - RGB
//  Second press - CMY
//  Third press - W
//  Repeat
// 6. If button released
//		Turn off the LED
//
//
void LED1_Init(void)
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

void LED2_Init(void)
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

void Switches_Init(void)
{
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL1 &= ~BIT1; //Select Port 1 Bit1 for use as port I/O
	P1->SEL0 &= ~BIT1; //Select Port 1 Bit1 for use as port I/O
	
	P1->SEL1 &= ~BIT4; //Select Port 1 Bit4 for use as port I/O
	P1->SEL0 &= ~BIT4; //Select Port 1 Bit4 for use as port I/O
	
	// configure as input
	P1->DIR &=~BIT1;  // set BIT 1 as input
	P1->DIR &=~BIT4;  // set BIT 4 as input
	
	//enable resistor
	P1->REN |=BIT1;
	P1->REN |=BIT4;
	
	//pull-up resistor
	P1->OUT |=BIT1;
	P1->OUT |=BIT4;
	
              
}
//------------Switch_Input------------
// Read and return the status of Switch1
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN 	Switch1_Pressed(void)									
{
	BOOLEAN retVal = FALSE;
	// check if pressed
		if((P1->IN & BIT1)==0) // If the pin reads as low (0) when the switch is closed (pressed) in an active-low configuration with pull-up resistor
		{retVal=TRUE;
		}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}
//------------Switch_Input------------
// Read and return the status of Switch2
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch2_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	// check if pressed
	if((P1->IN & BIT4)==0) // If the pin reads as low (0) when the switch is closed (pressed) in an active-low configuration with pull-up resistor
		{retVal=TRUE;
		}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}

	void delayMs (int delay)
	{ 
			for(int a =1; a <= 3200000; a++)
		{}
			
	}
int main(void){
	//Initialize any supporting variables
	int count=1;
	// Initialize buttons and LEDs
	LED1_Init();
	LED2_Init();
	Switches_Init();
	
	for(;;)
	{

		//Turn on an LED1 (RED) when button 1 is held
		 
			if(Switch1_Pressed() == TRUE)
		{    
			 P1->OUT |= BIT0; //Turn on an LED1 (RED)
				delayMs(1000);
		}
		else 
		{
			// turn off LED1
       P1->OUT &= ~ BIT0; //Turn OFF an LED1 (RED)
		}

		// now do the same for LED2
		
		if (Switch2_Pressed() == TRUE)//Button 2 pressed
		{	
			switch (count) {
			case 1: 
			P2->OUT  |=BIT0; //Turn ON LED Red
			delayMs(5000);	
			P2->OUT &=~BIT0; //Turn OFF LED Red
			
			P2->OUT  |=BIT1;  //Turn ON LED Green
			delayMs(5000);
			P2->OUT &=~BIT1; //Turn OFF LED Green
			
			P2->OUT  |=BIT2;	//Turn ON LED Blue 
			delayMs(5000);
			P2->OUT &=~BIT2;	//Turn OFF LED Blue
			count++;
			break;
			
			case 2:
			P2->OUT  |=(BIT1 | BIT2); //Turn ON LED CYAN
			delayMs(5000);	
			P2->OUT &=~(BIT1 |BIT2); //Turn OFF LED CYAN
			
			P2->OUT  |=(BIT0 | BIT2); //Turn ON LED MAGENTA
			delayMs(5000);	
			P2->OUT &=~(BIT0 |BIT2); //Turn OFF LED MAGENTA
			
			P2->OUT  |=(BIT0 | BIT1); //Turn ON LED Yellow
			delayMs(5000);	
			P2->OUT &=~(BIT0 |BIT1);	//Turn OFF LED Yellow
			count++;
			break;
			
			case 3:
			P2->OUT  |=(BIT0 | BIT1 | BIT2); //Turn ON LED WHITE
			delayMs(5000);	
			P2->OUT &=~(BIT0 | BIT1 | BIT2); //Turn OFF LED WHITE
			count=1;
			break;
		}	
	}
		else
		{
			// Turn off LED2
      P2->OUT &=~(BIT0 | BIT1 | BIT2); //Turn OFF LEDS

		}
		
		
	}
}

