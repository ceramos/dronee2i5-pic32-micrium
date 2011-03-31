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
#include <p32xxxx.h>
#include "ports.h"
#include "TaskAcq.h"
#include "global.h"
#include "encoder.h"
#include "drone.h"
#include "inputCapture.h"
#include "uart_transmit.h"


/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
OS_STK       AppTaskAcqStk[APP_TASK_ACQ_STK_SIZE];
byte mysamplebuff[500];

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
	drone.acq.myframe.data = mysamplebuff;
	drone.acq.myframe.idx = 0;
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

	//boolean définissant si l'on doit envoyer ou non la trame à cet instant
	BOOLEAN mustSendFrame = OS_FALSE;    

	TIMER1_Init(This->enc.fsmax);	// Use as Tick for the AcqTask
	
	while(1)
	{
		
		mustSendFrame = OS_FALSE;

		mPORTEClearBits(IOPORT_BIT_6);
		OSFlagPend(This->start_acq, (1 << 10), OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);	// Wait for the TIMER1 Tick		
		mPORTESetBits(IOPORT_BIT_6);
		
		if(err == OS_NO_ERR)
		{
			//--- Here we can start the acquisition
			This->enc.AcqSeq++;
			for(i = 0; i < NB_SENSOR; i++)			// Each of the sensor
			{
				ratio = This->enc.fsmax / drone.sensor[i].fs;
				if((This->enc.AcqSeq % ratio) == 0)		// Does the sensor have to sampling?
				{
					//On envoie la trame si le capteur courant est le capteur ayant la plus faible fréquence d'échantillonage
					//Ainsi la trame contient toujours les infos de tous les capteurs
					if(This->enc.fsmin == drone.sensor[i].fs)
					{
						mustSendFrame = OS_TRUE;
					}				

					// Retrieve informations from sensor
					sample.res = drone.sensor[i].sample.res;
					sample.offsetInFrame = drone.sensor[i].sample.offsetInFrame;
					sample.value = drone.sensor[i].get_sample();
					
					//copy into a frame
					compact(&sample, &This->myframe);
					//UART_Transmit_AddSample(&sample);
				}
			}
			//---------------------------------------------------//
			if(This->enc.AcqSeqCount == This->enc.AcqSeq )
	        {	
	         //length
	         	byte payload_length = This->myframe.idx;// - 2 pour AcqSeqCount
				//This->myframe.data[6] = HI_UINT16(payload_length);
				//This->myframe.data[7] = LO_UINT16(payload_length);
				UART_Transmit_SendFrame(This->myframe.data, payload_length);
				//acq->start_send(acq->mkf.data+5, acq->mkf.i-5 );
				This->enc.AcqSeq = 0;
				This->myframe.idx = 0;
			}
			//sends the frame
		/*	if(mustSendFrame == OS_TRUE)
			{
				UART_Transmit_SendFrame();
			}*/
		}
	}
}
