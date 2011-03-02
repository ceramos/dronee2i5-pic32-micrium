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
#include "TaskUartRx.h"
#include "global.h"
#include "encoder.h"
#include "drone.h"
#include "uart_header.h"

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
OS_STK       AppTaskUartRxStk[APP_TASK_UART_RX_STK_SIZE];

byte		 Trame_Recue_Uart[UART_RX_FRAME_SIZE];

byte		 Trame_Recue_Uart_Index = 0;

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static void AppTaskUartRx(void *p_arg);
static void analyseFrame();
static void clearFrame();
static void addByteToFrame(byte toAdd);
/*
*********************************************************************************************************
*                                               EXTERNES
*********************************************************************************************************
*/
extern byte	Uart_Rx_Circular_Buffer[CIRCULAR_BUFFER_SIZE];

extern byte	Circular_Buffer_Read_Index;
extern byte Circular_Buffer_Write_Index;

/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/
void create_uartRxtask(void)
{
	INT8U	err; 
	
	drone.uartRx.routine = AppTaskUartRx;
//	drone.uartRx.start_UartRx = OSFlagCreate(0x0000, &err);
/*	if(err != OS_NO_ERR)
	{
		//Erreur lors de la création de la tâche
		DBPRINTF("create_UartRxtask: err = %d\n", err);
	}*/
	OSTaskCreateExt(drone.uartRx.routine,
                   (void *)0,
                   (OS_STK *)&AppTaskUartRxStk[APP_TASK_UART_RX_STK_SIZE- 1],
                    APP_TASK_UARTRX_PRIO,
                    APP_TASK_UARTRX_PRIO,
                   (OS_STK *)&AppTaskUartRxStk[0],
                    APP_TASK_UART_RX_STK_SIZE,
                   (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}

/*
*********************************************************************************************************
*                                        UartRx  TASK
*
* Description : This task makes the green " of the card blink.
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/
static void AppTaskUartRx(void *p_arg)
{
	byte err, i, octetRecu;
	
	//contient les nouveaux octets recus depuis la dernière mise en pause de la tâche
	byte nouveauxOctets[CIRCULAR_BUFFER_SIZE];
	byte nombreOctetsRecus;
	
	TUartRx *This = &drone.uartRx;

	This->trameRecue.sof = UART_RX_FRAME_ID;
	This->trameRecue.size = UART_RX_FRAME_DATA_SIZE;
	This->trameRecue.eof = UART_RX_FRAME_DATA_SIZE^0xFF;

	//initialisation de la trame
	clearFrame();
	
	mPORTEOutputConfig(IOPORT_BIT_7);

	while(1)
	{
		OSTimeDlyHMSM(0, 0, 2, 0);
		mPORTEToggleBits(IOPORT_BIT_7);

		nombreOctetsRecus = 0;

		while(Circular_Buffer_Read_Index < Circular_Buffer_Write_Index)
		{
			nouveauxOctets[nombreOctetsRecus] = Uart_Rx_Circular_Buffer[Circular_Buffer_Read_Index];
			Circular_Buffer_Read_Index++;
			nombreOctetsRecus++;
		}

		while(Circular_Buffer_Read_Index > Circular_Buffer_Write_Index)
		{
			nouveauxOctets[nombreOctetsRecus] = Uart_Rx_Circular_Buffer[Circular_Buffer_Read_Index];
			Circular_Buffer_Read_Index++;
			nombreOctetsRecus++;

			if(Circular_Buffer_Read_Index == CIRCULAR_BUFFER_SIZE)
			{
				Circular_Buffer_Read_Index = 0;
			}
		}

		while(Circular_Buffer_Read_Index < Circular_Buffer_Write_Index)
		{
			nouveauxOctets[nombreOctetsRecus] = Uart_Rx_Circular_Buffer[Circular_Buffer_Read_Index];
			Circular_Buffer_Read_Index++;
			nombreOctetsRecus++;
		}

		for(i=0;i<nombreOctetsRecus;i++)
		{
			addByteToFrame(nouveauxOctets[i]);
		}
/*
		//on réccupère les octets recu et non lus
		if(Circular_Buffer_Read_Index < Circular_Buffer_Write_Index)
		{
			while(Circular_Buffer_Read_Index < Circular_Buffer_Write_Index)
			{
				
			}
		}
*/		
	}
}


/*
*********************************************************************************************************
*                                        UartRx  TASK
*
* Description : This task makes the green led of the card blink.
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/
static void addByteToFrame(byte toAdd)
{
	//Avant d'ajouter l'octet dans la trame, on vérifie que c'est celui attendu
	//S'il s'agit de l'octet SOF, ou SIZE
	if( (Trame_Recue_Uart_Index == OFFSET_TRAME_ID && toAdd == UART_RX_FRAME_ID) ||
		(Trame_Recue_Uart_Index == OFFSET_DATA_SIZE && toAdd == UART_RX_FRAME_DATA_SIZE) ||
		(Trame_Recue_Uart_Index >= OFFSET_MOTEUR_1) )
	{
		Trame_Recue_Uart[Trame_Recue_Uart_Index] = toAdd;
		Trame_Recue_Uart_Index++;

		if(Trame_Recue_Uart_Index >= UART_RX_FRAME_SIZE)
		{
			analyseFrame();
		}
	}
	else
	{
		//dans le cas ou l'octet recu n'est pas celui attendu, on sait que la trame n'est pas valide donc
		//on vide la trame et on recommence à zéro
		clearFrame();
	}
}


/*
*********************************************************************************************************
*                                        UartRx  TASK
*
* Description : This task makes the green led of the card blink.
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/
static void analyseFrame()
{
	byte vitesseMoteur1 = Trame_Recue_Uart[OFFSET_MOTEUR_1];
	byte vitesseMoteur2 = Trame_Recue_Uart[OFFSET_MOTEUR_2];
	byte vitesseMoteur3 = Trame_Recue_Uart[OFFSET_MOTEUR_3];
	byte vitesseMoteur4 = Trame_Recue_Uart[OFFSET_MOTEUR_4];

	//Dans un premier temps, on vérifie que la trame est valide en vérifiant la valeur des 
	//octets fixes (donc attendus)
	if( (Trame_Recue_Uart[OFFSET_TRAME_ID] == UART_RX_FRAME_ID) 		&&
		(Trame_Recue_Uart[OFFSET_DATA_SIZE] == UART_RX_FRAME_DATA_SIZE) &&
		(vitesseMoteur1 <= VITESSE_MAX_MOTEURS)							&&
		(vitesseMoteur2 <= VITESSE_MAX_MOTEURS)							&&
		(vitesseMoteur3 <= VITESSE_MAX_MOTEURS)							&&
		(vitesseMoteur4 <= VITESSE_MAX_MOTEURS)							&&
		(Trame_Recue_Uart[OFFSET_CHECK_VALID] == UART_RX_FRAME_CHECK_VALID) )
	{
		//La trame est bien valide, on assigne aux moteurs la valeur désirée
		drone.motor[MOTOR1-1].set_speed(MOTOR1, vitesseMoteur1);
		drone.motor[MOTOR2-1].set_speed(MOTOR2, vitesseMoteur2);
		drone.motor[MOTOR3-1].set_speed(MOTOR3, vitesseMoteur3);
		drone.motor[MOTOR4-1].set_speed(MOTOR4, vitesseMoteur4);
	}
	else
	{
		//TODO: traitement lorsque la trame n'est pas valide
	}

	clearFrame();
}

/*
*********************************************************************************************************
*                                        UartRx  TASK
*
* Description : This task makes the green led of the card blink.
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/
static void clearFrame()
{
	int i;
	for(i=0;i<UART_RX_FRAME_SIZE-1;i++)
	{
		Trame_Recue_Uart[i] = 0;
	}

	Trame_Recue_Uart_Index = 0;
}
