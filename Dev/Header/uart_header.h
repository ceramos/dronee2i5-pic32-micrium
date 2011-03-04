#ifndef _UART_HEADER_H
#define _UART_HEADER_H


/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/

#include "bsp_peripherals.h"
#include "includes.h"



/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/
#define CIRCULAR_BUFFER_SIZE	(UART_RX_FRAME_SIZE*3)	

#if (BSP_UART == BSP_UART1)

#define  BSP_UART_PUTCHAR(x)      putcUART1(x)
#define  BSP_UART_DATARDY()       DataRdyUART1()
#define  BSP_UART_GETCHAR()       ReadUART1()
#define  BSP_UART_INTCONFIG(x)    ConfigIntUART1(x)
#define  BSP_UART_START(x, y, z)  OpenUART1(x, y, z)
#define  BSP_UART_INTENABLE(x)    EnIntUART1(x)
#define  BSP_UART_INTDISABLE(x)   DisIntUART1(x)
#define  BSP_UART_INTEN           IEC0
#define  BSP_UART_INTSTAT         IFS0
#define  BSP_UART_INTCLR          IFS0CLR
#define  BSP_UART_INTMASK         0x1C000000
#define  BSP_UART_INTTX           0x10000000
#define  BSP_UART_INTRX           0x08000000
#define  BSP_UART_INTERR          0x04000000

#elif (BSP_UART == BSP_UART2)

#define  BSP_UART_PUTCHAR(x)      putcUART2(x)
#define  BSP_UART_DATARDY()       DataRdyUART2()
#define  BSP_UART_GETCHAR()       ReadUART2()
#define  BSP_UART_INTCONFIG(x)    ConfigIntUART2(x)
#define  BSP_UART_START(x, y, z)  OpenUART2(x, y, z)
#define  BSP_UART_INTENABLE(x)    EnIntUART2(x)
#define  BSP_UART_INTDISABLE(x)   DisIntUART2(x)
#define  BSP_UART_INTEN           IEC1
#define  BSP_UART_INTSTAT         IFS1
#define  BSP_UART_INTCLR          IFS1CLR
#define  BSP_UART_INTMASK         0x00000700
#define  BSP_UART_INTTX           0x00000400
#define  BSP_UART_INTRX           0x00000200
#define  BSP_UART_INTERR          0x00000100

#endif


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/
CPU_INT32U  UART_CalcBRG        (CPU_INT32U baud_rate);

void  UART_Config   (CPU_INT32U baud_rate);
void  UART_IntEn    (void);
void        UART_Init           (void);
CPU_CHAR    UART_RxByte         (void);
void        UART_TxByte         (CPU_CHAR c);
void        UART_TxStr          (CPU_CHAR  *str);

void        BSP_UARTHandler     (void);

#endif /*_UART_HEADER_H */
