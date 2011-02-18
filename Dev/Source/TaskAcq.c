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
#include <TaskAcq.h>
#include <global.h>
#include <encoder.h>
#include <app_cfg.h>


/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
OS_STK       AppTaskAcqStk[APP_TASK_ACQ_STK_SIZE];
TDrone drone;

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static void AppTaskAcq(void *p_arg);
static void init_drone(TDrone *This);
static void init_sensor(TSensor *This, byte resolution, INT16U freq);
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
		while(1)
		{
			LED_On(GREEN_LED);
				//mPORTDToggleBits(IOPORT_BIT_3);	
		} 	
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




static void init_drone(TDrone *This)
{
	byte i = 0;

	init_sensor(&This->sensor[i++], ACCEL_RES, ACCEL_FS);
	This->sensor[i++].get_sample = ADC_GetVal;

	init_sensor(&This->sensor[i++], ACCEL_RES, ACCEL_FS);
	This->sensor[i++].get_sample = ADC_GetVal;

	init_enc(&This->acq.enc, This->sensor);
}

static void init_sensor(TSensor *This, byte resolution, INT16U freq)
{
	This->sample.res = resolution;
	This->fs = freq;
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
	TSample  sample;
    TSensor* psensor;

	TIMER1_Init();
	
	init_drone(&drone);
	while(1)
	{
		LED_Toggle(GREEN_LED);
		OSFlagPend(drone.acq.start_acq, (1 << 10), OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);		
		if(err == OS_NO_ERR)
		{
			//--- Here we can start the acquisition
			LED_Toggle(ORANGE_LED);
			drone.acq.enc.AcqSeq++;
			for(i = 0; i < NB_SENSOR; i++)
			{
				ratio = drone.acq.enc.fsmax / drone.sensor[i].fs;
				if((drone.acq.enc.AcqSeq % ratio) == 0)
				{
					sample.res = drone.sensor[i].sample.res;
					//sample.value = drone.sensor[i].get_sample();
					// TODO: copy into a frame
				}
			}
		}
	}
}
