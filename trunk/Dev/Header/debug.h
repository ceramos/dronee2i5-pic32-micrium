/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/
#ifndef _DEBUG_H
#define _DEBUG_H

/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/

//#define DEBUG_DRONE

#ifdef DEBUG_DRONE

	#define DEBUG_ALTIMETER_DEFAUT_VALUE  23

	#define DEBUG_ACCELEROMETER_X_DEFAUT_VALUE  45
	#define DEBUG_ACCELEROMETER_Y_DEFAUT_VALUE  -125
	#define DEBUG_ACCELEROMETER_Z_DEFAUT_VALUE  1253
	
	#define DEBUG_GYRO_X_DEFAUT_VALUE	128
	#define DEBUG_GYRO_Y_DEFAUT_VALUE	-3011
	#define DEBUG_GYRO_Z_DEFAUT_VALUE	2048

#endif

#endif	// _DEBUG_H
