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
#ifndef _GLOBAL_H
#define _GLOBAL_H

/*
*********************************************************************************************************
*                                                INCLUDE
*********************************************************************************************************
*/
#include <ucos_ii.h>
#include <lib_def.h>

/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/
#define NB_SENSOR 	2
#define ACCEL_RES	14
#define ACCEL_FS	1000

typedef INT8U byte;

typedef void (*TPfct)(void *arg);
typedef INT16U (*TPfct1)(void);
typedef void (*TPfct2)(byte);
typedef void (*TPfct3)(byte, byte);


typedef struct{
	byte	sof;
	byte	size;
	byte* data;
	byte eof;
}TFrame; 



/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
extern OS_FLAG_GRP *flagLeds;
extern BOOLEAN orangeLedBlinking;

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#endif	// _GLOBAL_H



