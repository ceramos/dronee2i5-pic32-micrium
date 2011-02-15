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
#include <TaskOrangeLed.h>
#include <global.h>

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
OS_STK       AppTaskOrangeLedStk[APP_TASK_ORANGE_LED_STK_SIZE];

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*$PAGE*/
/*
*********************************************************************************************************
*                                          orange Led
*
* Description : This task makes the orange led of the card blink.
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppTaskOrangeLed(void *p_arg)
{
	/* Perform Initializations */
	  while(DEF_TRUE)
	  {
			if(orangeLedBlinking == OS_TRUE)
                LED_Toggle(ORANGE_LED);
            OSTimeDlyHMSM(0, 0, 0, 300);
       }
}
