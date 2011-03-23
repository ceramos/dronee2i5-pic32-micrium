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
#include <includes.h>
#include "TaskAcq.h"
#include "global.h"
#include "encoder.h"
#include "drone.h"
#include "inputCapture.h"


/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
OS_STK       AppTaskAcqStk[APP_TASK_ACQ_STK_SIZE];

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static void AppTaskAcq(void *p_arg);

/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/
void create_acqtask(void)
{
	INT8U	err; 
	
	drone.acq.routine = AppTaskAcq;
	drone.acq.start_acq = OSFlagCreate(0x0000, &err);
	if(err != OS_NO_ERR)
	{
		//Erreur lors de la création de la tâche
//		DBPRINTF("create_acqtask: err = %d\n", err);
	}
	OSTaskCreateExt(drone.acq.routine,
                   (void *)0,
                   (OS_STK *)&AppTaskAcqStk[APP_TASK_ACQ_STK_SIZE- 1],
                    APP_TASK_ACQ_PRIO,
                    APP_TASK_ACQ_PRIO,
                   (OS_STK *)&AppTaskAcqStk[0],
                    APP_TASK_ACQ_STK_SIZE,
                   (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}

/*
*********************************************************************************************************
*                                        Acq  TASK
*
* Description : This task makes the green led of the card blink.
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/
static void AppTaskAcq(void *p_arg)
{
	byte err, i, ratio;
	char str[10];
	TAcq *This = &drone.acq;
	TSample  sample;
    TSensor* psensor;
    
	TIMER1_Init();	// Use as Tick for the AcqTask

	while(1)
	{
		mPORTEToggleBits(IOPORT_BIT_6);

//		drone.motor[MOTOR1-1].set_speed(MOTOR1, 100);
		OSFlagPend(This->start_acq, (1 << 10), OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);	// Wait for the TIMER1 Tick		
		if(err == OS_NO_ERR)
		{
			//--- Here we can start the acquisition
			This->enc.AcqSeq++;
			for(i = 0; i < NB_SENSOR; i++)			// Each of the sensor
			{
				ratio = This->enc.fsmax / drone.sensor[i].fs;
				if((This->enc.AcqSeq % ratio) == 0)		// Does the sensor have to sampling?
				{
					sample.res = drone.sensor[i].sample.res;
					sample.value = drone.sensor[i].get_sample();	// Get the sample
					
					//sprintf(str, "%x\r\n", sample.value);		
					//UART_TxStr(str);
					// TODO: copy into a frame
				}
			}
		}
	}
}
