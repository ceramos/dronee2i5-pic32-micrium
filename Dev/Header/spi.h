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
* File : bma180.h
*********************************************************************************************************
*/
#ifndef _SPI_H
#define	 _SPI_H

/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/
#define CSB_BIT  BIT_14
#define SET_CSB   mPORTBSetBits(CSB_BIT)
#define CLEAR_CSB mPORTBClearBits(CSB_BIT) 

/*
*********************************************************************************************************
*                                                PROTOTYPES
*********************************************************************************************************
*/
void SPI_Init (void);
unsigned char SPI_read(unsigned char address);
void SPI_writeReg(unsigned char address, unsigned char data);
unsigned char Data_Received_SPI(void);
void Data_Send_SPI(unsigned char data_to_send);

#endif // _SPI_H
