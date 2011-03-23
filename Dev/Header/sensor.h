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
#ifndef _SENSOR_H
#define _SENSOR_H

/*
*********************************************************************************************************
*                                                INCLUDE
*********************************************************************************************************
*/
#include <global.h>


/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/
typedef struct{
	INT16U	value;
	byte	res;
	byte    offsetInFrame;
}TSample;

typedef struct{
	TSample	sample;
	INT16U	fs;
	TPfct1 get_sample;
}TSensor; 

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void init_sensor(TSensor *This, byte resolution, INT16U freq, byte offsetInFrame);

#endif	// _SENSOR_H
