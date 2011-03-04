/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2006, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            MIPS Sample code
*
* File : drone.c
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include <includes.h>
#include "drone.h"

#include "altimeter.h"


/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
TDrone drone;
/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/
/**
 * @brief: init a drone
 * @param: TDrone* This  Pointer to a drone struct
 * @return: void
 */
void init_drone(TDrone *This)
{
	byte i = 0;

	init_sensor(&This->sensor[i], ACCEL_RES, ACCEL_FS);
	This->sensor[i++].get_sample = &ADC_GetVal;

	init_sensor(&This->sensor[i], ACCEL_RES, ACCEL_FS);
	This->sensor[i++].get_sample = &ADC_GetVal;

	//Altimètre
	init_sensor(&This->sensor[i], ALTIMETER_RES, ALTIMETER_FS);
	This->sensor[i++].get_sample = &getAltitudeAsCentiMeter;
	

	init_enc(&This->acq.enc, This->sensor);
	for(i=0; i<4; i++)
	{
		init_motor(&This->motor[i], 5, i + MOTOR1);
	}
}
