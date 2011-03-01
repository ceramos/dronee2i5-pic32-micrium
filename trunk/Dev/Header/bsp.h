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
#ifndef		_BSP_H_
#define 	_BSP_H_
#include "bsp_peripherals.h"

/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/

#define  BSP_CLK_FREQ               72000000
//#define  JTAG_ENABLE                DEF_DISABLED
#define  JTAG_ENABLE                DEF_ENABLED
	
#define	 ALL_LED					0
#define	 GREEN_LED					1
#define  ORANGE_LED					2
#define  RED_LED					3

#define  PORT_GREEN_LED				IOPORT_BIT_2
#define  PORT_ORANGE_LED			IOPORT_BIT_1
#define  PORT_RED_LED				IOPORT_BIT_0

#define	 PORT_SW1					IOPORT_BIT_6
#define	 PORT_SW2					IOPORT_BIT_7
#define	 PORT_SW3					IOPORT_BIT_13

/*
*********************************************************************************************************
*                                             PROTOTYPES
*********************************************************************************************************
*/    

void        LED_Toggle          (CPU_INT08U led);
void        LED_Off             (CPU_INT08U led);
void        LED_On              (CPU_INT08U led);


void        BSP_ADCHandler      (void);
void        BSP_CNHandler       (void);
void        BSP_TickISR_Handler (void);
void        BSP_Except_Handler  (void);

void        BSP_InitIO          (void);
void        BSP_IntDisableALL   (void);

void        BSP_Dly             (CPU_INT32U  us);

CPU_INT16U  ADC_GetVal          (void);

void  TIMER1_Init (void);
void  TIMER2_Init (void);
#endif 	// _BSP_H_
