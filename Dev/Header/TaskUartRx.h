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
#ifndef _TASK_UART_RX_H
#define _TASK_UART_RX_H

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
#define APP_TASK_UART_RX_STK_SIZE	512	
	
typedef struct{	
	TPfct routine;
	TFrame trameRecue;
}TUartRx;

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
void create_uartRxtask(void);

#endif	//_TASK_UART_RX_H
