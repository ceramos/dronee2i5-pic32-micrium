#ifndef _UART_TRANSMIT_HEADER_H
#define _UART_TRANSMIT_HEADER_H


/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include "sensor.h"

/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/
#define TRANSMIT_BUFFER_SIZE	500//(UART_TX_FRAME_SIZE)
/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void UART_Transmit_InitDma(void);
void UART_Transmit_SendFrame(byte *datatosend, byte size);
void UART_Transmit_AddSample(TSample* sample);
void UART_Transmit_InitialiseFrame();


#endif /*_UART_TRANSMIT_HEADER_H */
