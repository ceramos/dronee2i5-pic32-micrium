/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/
#ifndef _GLOBAL_H
#define _GLOBAL_H

/*
*********************************************************************************************************
*                                                INCLUDE
*********************************************************************************************************
*/
#include <ucos_ii.h>
#include <lib_def.h>

/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/
#define NB_SENSOR 	2
#define ACCEL_RES	14
#define ACCEL_FS	1000

//Concernant l'altimètre
#define ALTIMETER_MICROSECOND_PER_INCH 147
#define CM_PER_INCH 2.54

//Concernant l'UART
#define UART_TRANSMIT_TRAME_SIZE 19


typedef INT8U byte;

typedef void (*TPfct)(void *arg);
typedef INT16U (*TPfct1)(void);
typedef void (*TPfct2)(byte);
typedef void (*TPfct3)(byte, byte);


typedef struct{
	byte	sof;
	byte	size;
	byte* data;
	byte eof;
}TFrame; 


typedef enum
{
	OFFSET_TRAME_ID=0,
	OFFSET_DATA_SIZE,
	OFFSET_MOTEUR_1,
	OFFSET_MOTEUR_2,
	OFFSET_MOTEUR_3,
	OFFSET_MOTEUR_4,
	OFFSET_CHECK_VALID,
	UART_RX_FRAME_SIZE
}TOffsetUartRxFrame;


//Trame réception: 7 octets: [ID(OxAA)]_[TAILLE_DATA(4)]_[MOTEUR_X(0-100)]_[TAILLE^^OxFF]
typedef enum
{
	UART_RX_FRAME_ID = 0xAA,
	UART_RX_FRAME_DATA_SIZE = 4,
	UART_RX_FRAME_CHECK_VALID = (UART_RX_FRAME_DATA_SIZE ^ 0xFF)
}TUartRxFrameConst;

#define VITESSE_MAX_MOTEURS 100

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
extern OS_FLAG_GRP *flagLeds;
extern BOOLEAN orangeLedBlinking;

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#endif	// _GLOBAL_H



