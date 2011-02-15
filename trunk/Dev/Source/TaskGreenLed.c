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
#include <includes.h>
#include <TaskGreenLed.h>
#include <global.h>

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
OS_STK       AppTaskGreenLedStk[APP_TASK_GREEN_LED_STK_SIZE];

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        Green  TASK
*
* Description : This task makes the green led of the card blink.
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/
void  AppTaskGreenLed(void *p_arg)
{
	/* Perform Initializations */
	  while(DEF_TRUE)
	  {
		INT8U err;
		OSFlagPend(flagLeds, (1 << 10), OS_FLAG_WAIT_SET_ALL +
		OS_FLAG_CONSUME, 0, &err);
            LED_Toggle(GREEN_LED);
       }
} 
