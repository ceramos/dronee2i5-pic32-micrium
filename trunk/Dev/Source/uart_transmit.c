/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include <plib.h>					// Peripheral Library

#include "encoder.h"
#include "uart_transmit.h"

/*²
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

static unsigned char	txferTxBuff[TRANSMIT_BUFFER_SIZE];	// the buffer to store the data to be received
byte indexUartTransmit = 0;

DmaChannel		dmaTxChn;	// DMA channel to use for our example

/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/

/**
* Description: This function initialises the Dma chanel for the uart transmit.
* Arguments  : none
* Returns    : none
*/
void UART_Transmit_InitDma()
{
	dmaTxChn=DMA_CHANNEL1;	// DMA channel to use for our example

	DmaChnOpen(dmaTxChn, DMA_CHN_PRI3, DMA_OPEN_DEFAULT);

	// set the events: we want the SPI receive buffer full interrupt to start our transfer
	DmaChnSetEventControl(dmaTxChn, DMA_EV_START_IRQ_EN|DMA_EV_START_IRQ(_UART2_TX_IRQ));

	// set the transfer:
	// source is the SPI buffer, dest is our memory buffer
	// source size is one byte, destination size is the whole buffer
	// cell size is one byte: we want one byte to be sent per each SPI RXBF event
	DmaChnSetTxfer(dmaTxChn, txferTxBuff, (void*)&U2TXREG, sizeof(txferTxBuff), 1, 1);

	DmaChnEnable(dmaTxChn);	// enable the DMA channel
}


/**
* Description: This function sends the frame to the uart
* Arguments  : none
* Returns    : none
*/
void UART_Transmit_SendFrame()
{
	DmaTxferRes  res = DmaChnStartTxfer(dmaTxChn, DMA_WAIT_BLOCK, 1);
	UART_Transmit_InitialiseFrame();
//UART_TxByte('4');
}


/**
* Description: This function adds a sample into the frame
* Arguments  : sample
* Returns    : none
*/
void UART_Transmit_AddSample(TSample *sample)
{
	byte toAdd;
	byte i;
	byte resolutionAsByte = sample->res/8 + 1;

	if(resolutionAsByte == 2)
	{
		toAdd = (sample->value & 0xff00) >> 8;
		txferTxBuff[indexUartTransmit++] = toAdd; 		
	}

	toAdd = sample->value & 0x00ff;
	txferTxBuff[indexUartTransmit++] = toAdd;
}


/**
* Description: This function adds a sample into the frame
* Arguments  : sample
* Returns    : none
*/
void UART_Transmit_InitialiseFrame()
{
	indexUartTransmit = OFFSET_FIRST_SENSOR;
	int i = 0;
	for(i = 0; i<TRANSMIT_BUFFER_SIZE; i++)
	{
		txferTxBuff[i] = 0;
	}

	txferTxBuff[OFFSET_TRAME_ID] = UART_TX_FRAME_ID;
	txferTxBuff[OFFSET_DATA_SIZE] = UART_TX_FRAME_DATA_SIZE;
	txferTxBuff[OFFSET_TX_CHECK_VALID] = UART_TX_FRAME_CHECK_VALID;
}
