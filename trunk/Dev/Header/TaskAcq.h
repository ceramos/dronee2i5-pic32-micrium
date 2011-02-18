/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                             (c) Copyright 2006, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                          Board Support Package
*                                               Microchip
*                                              Explorer 16
*********************************************************************************************************
*/
#ifndef _TASKACQ_H
#define _TASKACQ_H

/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include <global.h>
#include <encoder.h>

/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/
#define APP_TASK_ACQ_STK_SIZE	512	
	
typedef struct{	
	TPfct routine;
	OS_FLAG_GRP* start_acq;
	TEncoder enc;
	TFrame trame;
}TACQ;

typedef struct{
	TACQ acq;
	TSensor sensor[NB_SENSOR];
}TDrone;
	

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
//extern OS_STK       AppTaskAcqStk[APP_TASK_ACQ_STK_SIZE];
//extern TACQ acq;
extern TDrone drone;

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void create_acqtask(void);

#endif	//_TASKACQ_H
