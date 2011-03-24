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
//fréquence de la tâche d'acquisition
#define ACQUISITION_TASK_FREQUENCY ACCEL_FS

#define ACCEL_RES	14
#define ACCEL_RES_BYTE	     (ACCEL_RES%8==0? ACCEL_RES/8: 1 + ACCEL_RES/8)	
#define ACCEL_FS	500

#define ALTIMETER_RES	8
#define ALTIMETER_RES_BYTE	(ALTIMETER_RES%8==0? ALTIMETER_RES/8: 1 + ALTIMETER_RES/8)
#define ALTIMETER_FS	25

#define GYRO_RES	16
#define GYRO_RES_BYTE		(GYRO_RES%8==0? GYRO_RES/8: 1 + GYRO_RES/8)

#define BATTERY_RES  8
#define BATTERY_RES_BYTE		(BATTERY_RES%8==0? BATTERY_RES/8: 1 + BATTERY_RES/8)

//Concernant l'altimètre
#define ALTIMETER_MICROSECOND_PER_INCH 147
#define CM_PER_INCH 2.54


typedef INT8U byte;

typedef void (*TPfct)(void *arg);
typedef short (*TPfct1)(void);
typedef void (*TPfct2)(byte);
typedef void (*TPfct3)(byte, byte);
typedef short (*TPfct4)(byte);


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
	OFFSET_RX_CHECK_VALID,
	UART_RX_FRAME_SIZE
}TOffsetUartRxFrame;


//Trame réception: 7 octets: [ID(OxAA)]_[TAILLE_DATA(4)]_[MOTEUR_X(0-100)]_[TAILLE^^OxFF]
typedef enum
{
	UART_RX_FRAME_ID = 0xAA,
	UART_RX_FRAME_DATA_SIZE = 4,
	UART_RX_FRAME_CHECK_VALID = (UART_RX_FRAME_DATA_SIZE ^ 0xFF)
}TUartRxFrameConst;

//Trame émission: : [ID(OxAA)]_[TAILLE_DATA(4)]_[MOTEUR_X(0-100)]_[TAILLE^^OxFF]
typedef enum
{
	UART_TX_FRAME_ID = 0x88,
	UART_TX_FRAME_DATA_SIZE = ACCEL_RES_BYTE+ACCEL_RES_BYTE+ACCEL_RES_BYTE+GYRO_RES_BYTE+GYRO_RES_BYTE+GYRO_RES_BYTE+ALTIMETER_RES_BYTE+BATTERY_RES_BYTE,
	UART_TX_FRAME_CHECK_VALID = (UART_TX_FRAME_DATA_SIZE ^ 0xFF)
}TUartTxFrameConst;


typedef enum
{
	OFFSET_ACCEL_X	 	  =		2,
	OFFSET_ACCEL_Y   	  =		OFFSET_ACCEL_X		+ ACCEL_RES_BYTE,
	OFFSET_ACCEL_Z    	  =		OFFSET_ACCEL_Y   	+ ACCEL_RES_BYTE,
	OFFSET_GYRO_X 	 	  =		OFFSET_ACCEL_Z   	+ ACCEL_RES_BYTE,
	OFFSET_GYRO_Y 	 	  =		OFFSET_GYRO_X    	+ GYRO_RES_BYTE,
	OFFSET_GYRO_Z 	 	  =		OFFSET_GYRO_Y 		+ GYRO_RES_BYTE,
	OFFSET_ALTIMETER 	  =		OFFSET_GYRO_Z    	+ GYRO_RES_BYTE,
	OFFSET_BATTERY   	  = 	OFFSET_ALTIMETER 	+ ALTIMETER_RES_BYTE,
	OFFSET_TX_CHECK_VALID = 	OFFSET_BATTERY 		+ BATTERY_RES_BYTE,
	UART_TX_FRAME_SIZE
}TOffsetUartTxFrame;

#define VITESSE_MAX_MOTEURS 100

#define RESOLUTION_1_BYTE  = 8;
#define RESOLUTION_2_BYTE  = RESOLUTION_1_BYTE * 2;
#define RESOLUTION_3_BYTE  = RESOLUTION_1_BYTE * 3;

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

#endif	// _GLOBAL_H



