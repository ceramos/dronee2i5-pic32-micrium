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
#include <TaskRedLed.h>

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
OS_STK       AppTaskRedLedStk[APP_TASK_RED_LED_STK_SIZE];

/*$PAGE*/
/*
*********************************************************************************************************
*                                          Red Led
*
* Description : This task makes the red led of the card blink.
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppTaskRedLed(void *p_arg)
{
	/* Perform Initializations */
	  while(DEF_TRUE)
	  {
                LED_Toggle(RED_LED);
                OSTimeDlyHMSM(0, 0, 0, 200);
       }
}    
