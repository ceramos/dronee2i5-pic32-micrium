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
* File : APP.C
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include <encoder.h>
#include <global.h>

#include "drone.h"
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
void init_enc(TEncoder *This, TSensor *sensor)
{
	byte i;
	This->fsmin = 65535;
	This->fsmax = 0;
	
	This->AcqSeq = 0;
	for(i = 0; i < NB_SENSOR;	i++)
	{
		if(sensor[i].fs < This->fsmin)
			This->fsmin = sensor[i].fs;
		if(sensor[i].fs > This->fsmax)
			This->fsmax = sensor[i].fs;
		This->AcqSeqCount = This->fsmax/This->fsmin;
	}
}


void compact(TSample *sample, TFrame *frame)
{
}

/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/
