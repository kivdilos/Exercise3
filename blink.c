//******************************************************************************
//  MSP430x2xx Demo - Read button status and togle LED
//
//  Description; Toggle LEDs according to button commands inside a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x2xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|--> RED LED
//            |             P1.1|--> GREEN LED
//            |             P1.2|--> Button switch S1
//                                   (push=GND=logic 0!!)
//
//
// Original source: A. Dannenberg  Texas Instruments, Inc  January 2006
// Modified for eZ430_RF2500 LED blink demo: Gregory Doumenis,
//                                 TEIEP, Oct. 2011, Nov. 2012
//  Built with CCE for MSP430 Version: 4.2.4
//******************************************************************************

#include "msp430f2101.h"


// Exercise 1: Write delay function delay_i( unsigned int x )


void delay_i( unsigned int x ) // delay function
{
	volatile unsigned int j=x;	// without volatile doesn't work
	do j--;						// decrease j
	while(j != 0);				//
}

void freeze()			// freeze function
{

//	do{
//	P1OUT ^= 0x03;		// xor leds
//	}while((P1IN & 0x04) != 0);
}

void blinkfast()
{
	P1OUT ^= 0x03;
	delay_i(25000);
}

void blinkleds(volatile unsigned int v)		// blinkleds function
{
	if(v==0)								// if value is 0 which it is as tmp starts from 0
	{
//		do{
		P1OUT ^= 0x03;						// xor both leds
		delay_i(50000);						// and wait ~1 second
//		}while((P1IN & 0x04) != 0);
	}
	else									// if value is 1
	{
//		do{
		P1OUT ^= 0x03;						// xor both leds
		delay_i(25000);						// wait for ~half second
		P1OUT ^= 0x02;						// xor green led
		delay_i(25000);						// and wait another half second
//		}while((P1IN & 0x04) != 0);
	}
}

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  P1DIR |= 0x03;                        // Set P1.0 & P1.1 to output direction
  P1OUT = 0x04;                         // Dummy, to set P1.2 pullup (S1 grounds P1.2!!)
  P1REN |= 0x04;                        // sets P1.2 pullup


    volatile unsigned int B_pressed=0;           // volatile to prevent optimization -
    volatile unsigned int Led_Blink_on=0;
    volatile unsigned int B_sense=0;
    volatile unsigned int i=0;
    volatile unsigned int tmp=0;
    volatile unsigned int k=25000;				// ~half second time period for exercise 4
    volatile unsigned int r=0;					// used for button counter

// Exercise 2: Detect the presence of button and blink RED LED for ~1 sec

// Exercise 3: Start and stop (freeze) LED blink sequence
// according to button pressed. When unfreezed, Green LED blinks
// twice as fast as RED


// Exercise 4: Start and stop (freeze) RED LED blink sequence
// according to button pressed. If button is pressed twice
//(momentarily), then double the blink frequency.

  for (;;)
  {
	  B_sense = P1IN & 0x04;
	 /* do{						// check how many times button pressed in half second
		  if(B_sense == 0)		// detect press of button
		  {
			  do
				  B_sense = P1IN & 0x04;
			  while(B_sense == 0);
			  r++;				// if pressed add r + 1
		  }
		  k--;
	  }while((k != 0) & (r != 2));			// while half second not passed and not pressed 2 times
	  k=25000;					// give back ~half second
	  if (r == 2)				// if button pressed 2 times
	  {
		  do{					// not yet sure
			  blinkfast();
			  B_sense = P1IN & 0x04;	// continuously check if button is pressed
		  }while(B_sense != 0);
		  r=0;					// make r 0 for next counting
	  }*/

   if (B_sense==0) //debounce
      {
	   	   do
	   	   {
	   		   B_sense = P1IN & 0x04;	// continuously check if button is pressed

	   	   }while(B_sense == 0);		// and exit the loop when button unpressed

	   	   B_pressed ^= 1;				// change the B_pressed by xoring it
	   	   if(B_pressed == 1)			// if it is 1
	   	   {
	   		   Led_Blink_on = 1;		// change variable value to 1
	   		   //tmp = 0;
	   	   }
	   	   else							// if not
	   	   {
	   		   Led_Blink_on = 0;		// change variable value to 0
	   		   tmp ^= 1;				// by xoring tmp we tell blinkleds function to what frequency to blink them
	   	   }

      }
   else									// if b_sense = 0 which it is after button is unpressed
   {
	   if(Led_Blink_on == 1)			// if led blink is on
	   {
		   do{
			   B_sense = P1IN & 0x04;
			   blinkleds(tmp);				// blink leds according to tmp
	   }while(B_sense != 0);
	   }
	   else								// or else
	   {
		   do{
			   B_sense = P1IN & 0x04;
			   freeze();				// freeze leds
		   }while(B_sense != 0);
	   }
   }
 }

}

