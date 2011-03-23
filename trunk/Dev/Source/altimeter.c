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

/*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include <plib.h>

#include "global.h"
#include "altimeter.h"
#include "inputCapture.h"
#include "debug.h"
/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
extern unsigned short int ecart;	

/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/

/**
 * @brief: get the altitude of the drone as Inchs
 * @param: void
 * @return: the altitude
 */
CPU_INT16U getAltitudeAsInch()
{
	return (((float)getHighStateTime()) / ALTIMETER_MICROSECOND_PER_INCH);
}

/**
 * @brief: get the altitude of the drone as cm
 * @param: void
 * @return: the altitude
 */
short getAltitudeAsCentiMeter()
{
	CPU_INT16U altitude;

	mPORTDSetBits(BIT_2);


	//Pour tester l'emmission UART, on envoie une valeur par défaut
	#ifdef DEBUG_ALTIMETER_DEFAUT_VALUE
		altitude = DEBUG_ALTIMETER_DEFAUT_VALUE;
	#else
		altitude = getAltitudeAsInch() * CM_PER_INCH;
	#endif

	//Etant donné que la résolution du capteur est sur 8 bits, on cast l'altitude sur 8 bits
	if(ALTIMETER_RES <= 8 && altitude > 255)
	{
		altitude = 255;
	}

	mPORTDClearBits(BIT_2);

	return altitude;
}

/**
 * @brief: get the high state time of the altimeter
 * @param: void
 * @return: the high state time
 */
CPU_INT16U getHighStateTime()
{
	return (int)(ecart * MICROSECOND_PER_TIMER3_TICK);
}
