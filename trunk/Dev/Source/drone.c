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
#include "bma180.h"


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
	
	//Accelerom�tre
	init_sensor(&This->sensor[i], ACCEL_RES, ACCEL_FS, OFFSET_ACCEL_X);
	This->sensor[i++].get_sample = &Accel_getX;

	init_sensor(&This->sensor[i], ACCEL_RES, ACCEL_FS, OFFSET_ACCEL_Y);
	This->sensor[i++].get_sample = &Accel_getY;
	
	init_sensor(&This->sensor[i], ACCEL_RES, ACCEL_FS, OFFSET_ACCEL_Z);
	This->sensor[i++].get_sample = &Accel_getZ;

	//Altim�tre
	init_sensor(&This->sensor[i], ALTIMETER_RES, ALTIMETER_FS, OFFSET_ALTIMETER);
	This->sensor[i++].get_sample = &getAltitudeAsCentiMeter;
	

	init_enc(&This->acq.enc, This->sensor);
	for(i=0; i<4; i++)
	{
		init_motor(&This->motor[i], 5, i + MOTOR1);
	}
}