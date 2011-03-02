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
* File : sensor.c
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include <includes.h>
#include <encoder.h>
#include <global.h>

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


/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/
void init_sensor(TSensor *This, byte resolution, INT16U freq)
{
	This->sample.res = resolution;
	This->fs = freq;
}