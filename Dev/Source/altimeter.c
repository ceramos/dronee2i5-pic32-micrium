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
#include "altimeter.h"
#include "inputCapture.h"

extern unsigned short int ecart;	


//BSP_CLK_FREQ 
CPU_INT16U getAltitudeAsInch()
{
	return (((float)getHighStateTime()) / ALTIMETER_MICROSECOND_PER_INCH);
}

short getAltitudeAsCentiMeter()
{
	CPU_INT16U altitude;

	mPORTDSetBits(BIT_2);
	altitude = getAltitudeAsInch() * CM_PER_INCH;
	mPORTDClearBits(BIT_2);

	return altitude;
}

CPU_INT16U getHighStateTime()
{
	return (int)(ecart * MICROSECOND_PER_TIMER3_TICK);
}
