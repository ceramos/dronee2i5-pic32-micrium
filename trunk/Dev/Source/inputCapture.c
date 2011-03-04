/*********************************************************************
 *
 *                  OCMP Simple INPUT_CAPTURE Application
 *
 *********************************************************************
 * FileName:        ocmp_simple_pwm.c
 * Dependencies:
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 *********************************************************************
 *
 * This Program Demonstrates continuous Pulse generation on
 * Output compare module1 of the PIC32 controller.
 *
 * The H/W setup contains PIC32 placed on an Explorer16 board.
 * Observe the output pulse on OC1 pin using an Oscilloscope.
 *
 **********************************************************************/
#include <plib.h>

#include "global.h"
#include "inputCapture.h"



unsigned short int risingEdgeValue = 0;
unsigned short int fallingEdgeValue = 0;


unsigned short int ecart = 0;	

/**
* Description: This function calculates the correct multiplier for the selected baud rate.
* Arguments  : baud_rate    The desired baud rate for the UART
* Returns    : The calcualted multiplier for the selected baud rate.
*/
void INPUT_CAPTURE_Init(void)
{
    PORTSetPinsDigitalOut(IOPORT_D, BIT_1|BIT_2);

	//Input capture ???? ???
	OpenTimer3(T3_ON | T3_PS_1_256, 0xffff);
    // Enable Input Capture Module 1 - RD8
    // - Capture Every edge
    // - Enable capture interrupts
    // - Use Timer 3 source
    // - Capture rising edge first
	OpenCapture1( IC_EVERY_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_FEDGE_FALL | IC_ON );
	mIC1IntEnable(1);
	mIC1SetIntPriority(2);
}

/**
* Description: This function calculates the correct multiplier for the selected baud rate.
* Arguments  : baud_rate    The desired baud rate for the UART
* Returns    : The calcualted multiplier for the selected baud rate.
*/
void INPUT_CAPTURE_Stop()
{
	CloseCapture1();
	CloseTimer3();
}



/*
*********************************************************************************************************
*                                    BSP_INPUT_CAPTURE1Handler()
*
* Description: This function handles interrupts from the UART.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/
void  BSP_INPUT_CAPTURE1Handler (void)
{
	if(PORTDbits.RD8 == 0)
	{	
		if(mIC1CaptureReady())
			fallingEdgeValue = mIC1ReadCapture();
		else 
			fallingEdgeValue = 0;

		if(fallingEdgeValue > risingEdgeValue)
		{
			ecart = fallingEdgeValue - risingEdgeValue;
		}
		else
		{
			ecart = 0xFFFF - risingEdgeValue;
			ecart += fallingEdgeValue;
	//		ecart = fallingEdgeValue - risingEdgeValue;
	//		ecart = ~ecart;
			mPORTDSetBits(BIT_1);			
		}
	}
	else
	{
		//On met le compteur à 0
		risingEdgeValue = mIC1ReadCapture();
	}
	mIC1ClearIntFlag();
}
