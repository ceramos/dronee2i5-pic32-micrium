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
#include <TaskRedLed.h>
#include <TaskOrangeLed.h>
//#include <task_cfg.h>

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

static  OS_STK       AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK       AppTaskUartTransmitStk[APP_TASK_UART_TRANSMIT_STK_SIZE];

/* declaration of flag group */
OS_FLAG_GRP *flagLeds;

BOOLEAN orangeLedBlinking = OS_FALSE;

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void        AppTaskStart(void *p_arg);
static  void        AppTaskCreate(void);
static  void		AppTaskUartTransmit(void *p_arg);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.
*
* Arguments   : none
*********************************************************************************************************
*/

#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_18, FPLLIDIV = DIV_2, FWDTEN = OFF, FPBDIV = DIV_1, POSCMOD = XT, FNOSC = PRIPLL, CP = OFF

int  main (void)
{
    CPU_INT08U  err;


    BSP_IntDisAll();                                                    /* Disable all interrupts until we are ready to accept them */

    OSInit();                                                           /* Initialize "uC/OS-II, The Real-Time Kernel"              */

    OSTaskCreateExt(AppTaskStart,                                       /* Create the start task                                    */
                    (void *)0,
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppTaskStartStk[0],
                    APP_TASK_START_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_SIZE > 13
    OSTaskNameSet(APP_TASK_START_PRIO, "Startup", &err);
#endif

	
	/* within main programm, between OSInit() and OSStart() */
	flagLeds = OSFlagCreate(0x0000, &err);


    OSStart();          //the last function called from main                                                /* Start multitasking (i.e. give control to uC/OS-II)       */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
#if CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL
    CPU_SR  cpu_sr;
#endif
    CPU_INT08U  i;
    CPU_INT08U  j;

    (void)p_arg;

    BSP_InitIO();                                                       /* Initialize BSP functions                                 */

#if PROBE_COM_METHOD_RS232 == DEF_FALSE
    UART_Init();                                                        /* Initialize serial port                                   */
#endif

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                                       /* Determine CPU capacity                                   */
#endif
/*
#if (uC_PROBE_OS_PLUGIN > 0) || (uC_PROBE_COM_MODULE > 0)
    AppProbeInit();                                                     /* Initialize uC/Probe modules                              */
//#endif
    
    AppTaskCreate();                                                    /* Create application tasks                                 */
    
    while (OS_TRUE) {                                                   /* Task body, always written as an infinite loop.            */
                OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                        CREATE APPLICATION TASKS
*
* Description: This function creates the application tasks.
*
* Arguments  : none.
*
* Note(s)    : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    CPU_INT08U  err;
   
    OSTaskCreateExt(AppTaskGreenLed,
                   (void *)0,
                   (OS_STK *)&AppTaskGreenLedStk[APP_TASK_GREEN_LED_STK_SIZE - 1],
                    APP_TASK_GREEN_LED_PRIO,
                    APP_TASK_GREEN_LED_PRIO,
                   (OS_STK *)&AppTaskGreenLedStk[0],
                    APP_TASK_GREEN_LED_STK_SIZE,
                   (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
                    
    OSTaskNameSet(APP_TASK_GREEN_LED_PRIO, (INT8U *)"Green Led", &err);

    OSTaskCreate(AppTaskOrangeLed,
                   (void *)0,
                   (OS_STK *)&AppTaskOrangeLedStk[APP_TASK_ORANGE_LED_STK_SIZE - 1],
                    APP_TASK_ORANGE_LED_PRIO);

    OSTaskNameSet(APP_TASK_ORANGE_LED_PRIO, (INT8U *)"Orange Led", &err);

    OSTaskCreate(AppTaskRedLed,
                   (void *)0,
                   (OS_STK *)&AppTaskRedLedStk[APP_TASK_RED_LED_STK_SIZE - 1],
                    APP_TASK_RED_LED_PRIO);

    OSTaskNameSet(APP_TASK_RED_LED_PRIO, (INT8U *)"Red Led", &err);


    OSTaskCreate(AppTaskUartTransmit,
                   (void *)0,
                   (OS_STK *)&AppTaskUartTransmitStk[APP_TASK_UART_TRANSMIT_STK_SIZE - 1],
                    APP_TASK_UART_TRANSMIT_PRIO);

    OSTaskNameSet(APP_TASK_UART_TRANSMIT_PRIO, (INT8U *)"Uart Transmit", &err);


}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          Uart Transmit
*
* Description : This task sends data to the UART. Wait for data with a messageBox
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppTaskUartTransmit(void *p_arg)
{
	/* Perform Initializations */
	  while(DEF_TRUE)
	  {
		
            OSTimeDlyHMSM(0, 0, 0, 300);
	
       }
}    
