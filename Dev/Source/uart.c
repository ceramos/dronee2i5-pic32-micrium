/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include "uart_header.h"
#include "uart_transmit.h"
#include "uart.h"
#include "global.h"


/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
byte		 Uart_Rx_Circular_Buffer[CIRCULAR_BUFFER_SIZE];

byte		 Circular_Buffer_Read_Index  = 0;
byte		 Circular_Buffer_Write_Index = 0;

CPU_BOOLEAN	 Circular_Buffer_Overflow = OS_FALSE;


/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/

/**
* Description: This function calculates the correct multiplier for the selected baud rate.
* Arguments  : baud_rate    The desired baud rate for the UART
* Returns    : The calcualted multiplier for the selected baud rate.
*/
CPU_INT32U  UART_CalcBRG (CPU_INT32U baud_rate)
{
    CPU_INT32U  brg;
    
    brg = BSP_CLK_FREQ / (16 * baud_rate) - 1;
    
    return (brg);
}


/**
* Description: This function configures and enables interrupts for the UART.
* Arguments  : None
* Returns    : None
*/
void  UART_IntEn (void)
{
    CPU_INT08U  config;
        
    config = UART_INT_PR3
           | UART_INT_SUB_PR0
           | UART_RX_INT;
           
    BSP_UART_INTCONFIG(config);
}


/**
* Description: This function performs all necessary configurations on the UART.
* Arguments  : baud_rate    The desired baud rate for the UART
* Returns    : None
*/
void  UART_Config (CPU_INT32U baud_rate)
{    
    CPU_INT32U  ubrg;
    CPU_INT32U  config1;
    CPU_INT32U  config2;
    
  
    mPORTAClearBits(DEF_BIT_07);                                        /* Turn off RA7 on startup.                         */
    mPORTASetPinsDigitalOut(DEF_BIT_07);                                /* Make RA7 as output.                              */
    
    ubrg    = UART_CalcBRG(baud_rate);                                  /* Calculate the correct multiplier                 */
    
    config1 = UART_EN                                                   /* UART module enabled                              */
            | UART_IDLE_CON                                             /* UART works in IDLE mode                          */
            | UART_RX_TX                                                /* Communication is done through the normal pins    */
            | UART_DIS_WAKE                                             /* Disable Wake-up on START bit detect during SLEEP */
            | UART_DIS_LOOPBACK                                         /* Disable loop back                                */
            | UART_DIS_ABAUD                                            /* Input to capture module from ICx pin             */
            | UART_NO_PAR_8BIT                                          /* 8 bits no parity                                 */
            | UART_1STOPBIT                                             /* 1 stop bit                                       */
            | UART_IRDA_DIS                                             /* IrDA disabled                                    */
            | UART_MODE_FLOWCTRL                                        /* UART pins in flow control mode                   */
            | UART_DIS_BCLK_CTS_RTS                                     /* Disable BCLK, CTS, and RTS pins                  */
            | UART_NORMAL_RX                                            /* UxRX idle stat is '1'                            */
            | UART_BRGH_SIXTEEN;                                        /* 16x baud clock                                   */
            
    config2 = UART_TX_PIN_LOW                                           /* IrDA encoded UxTx idle stat is '0'               */
            | UART_RX_ENABLE                                            /* Enable UxRx pin                                  */
            | UART_TX_ENABLE                                            /* Enable UxTx pin                                  */
            | UART_INT_TX                                               /* Interrupt on trasnfer of each character to TSR   */
            | UART_INT_RX_CHAR                                          /* Interrupt on every char received                 */
            | UART_ADR_DETECT_DIS                                       /* Disable 9-bit address detect                     */
            | UART_RX_OVERRUN_CLEAR;                                    /* Rx buffer overrun status bit clear               */
            
    BSP_UART_START(config1, config2, ubrg);                             /* Configure the settings                           */
}


/**
* Description: This function performs all necessary initializations on the UART.
* Arguments  : baud_rate    The desired baud rate for the UART
* Returns    : None
*/
void  UART_Init (void)
{
	int i;
	//initialisation des tableaux
	for(i=0; i<CIRCULAR_BUFFER_SIZE-1; i++)
	{
		Uart_Rx_Circular_Buffer[i] = 0;
	}

    UART_Config(BSP_BAUD_RATE);
    UART_IntEn();

	//Concerne la transmission uart
	UART_Transmit_InitDma();
	UART_Transmit_InitialiseFrame();
}

/**
* Description: This function receives a single character.
* Arguments  : None
* Returns    : The character that is read. 
* Note(s)    : This function calls OSTimeDly, it should not be used in ISRs.
*/
CPU_CHAR  UART_RxByte (void)
{
    while (BSP_UART_DATARDY() == DEF_FALSE) {
        OSTimeDly(1);                                                   /* If there is no data to be read, delay            */
    }
        
    return (BSP_UART_GETCHAR());                                        /* Return a character                               */
}



/**
* Description: This function performs all necessary initializations on the UART.
* Arguments  : baud_rate    The desired baud rate for the UART
* Returns    : None
*/  
void  UART_TxByte (CPU_CHAR c)
{
	BSP_UART_PUTCHAR(c);                                            /* Output the character                             */
}    


/**
* Description: This function transmits a string using the UART.
* Arguments  : str          A pointer the the location of the string to be output.
* Returns    : None
*/
void  UART_TxStr (CPU_CHAR  *str)
{
    while (*str) {
        UART_TxByte(*str++);              
    }
} 

/**
* Description: This function handles interrupts from the UART and insert the received byte into the buffer
* Arguments  : None
* Returns    : None
*/
void  BSP_UARTHandler (void)
{
    CPU_INT32U  status;
    CPU_INT32U  int_en;
    
    status           = BSP_UART_INTSTAT;                                /* Read the interrupt status register               */
    int_en           = BSP_UART_INTEN;                                  /* Read the interrupt enabled register              */
    
    status          &= BSP_UART_INTMASK;                                /* Mask all other interrupts                        */
    BSP_UART_INTCLR  = status;                                          /* Clear all triggered interrupts                   */
    status          &= int_en;                                          /* Mask non-enabled interrupts                      */
    
    if (status & BSP_UART_INTRX)
	{ 
	   //If a Rx interrupt has occured and is enabled...
	   //On stocke l'octet recu dans le buffer circulaire
	   Uart_Rx_Circular_Buffer[Circular_Buffer_Write_Index] = UART_RxByte();
		
		//On incrémente l'index d'écriture du buffer	
		if(Circular_Buffer_Write_Index >= (CIRCULAR_BUFFER_SIZE - 1))
		{
			Circular_Buffer_Write_Index = 0;	
		}
		else
		{
			Circular_Buffer_Write_Index++;
		}
		
		//Si l'index d'écriture est égal à l'index de lecture, c'est que le buffer est plein (on vient de faire un tour)
		if(Circular_Buffer_Write_Index == Circular_Buffer_Read_Index)
		{
			//TODO: gérer le buffer overflow
			Circular_Buffer_Overflow = OS_TRUE;
		}
	}
}
