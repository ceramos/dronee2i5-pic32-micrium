/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include <plib.h>					// Peripheral Library

#include "encoder.h"
#include "uart_transmit.h"

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

static unsigned char	dmabuff[TRANSMIT_BUFFER_SIZE];	// the buffer to store the data to be received

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
	DmaChnSetTxfer(dmaTxChn, dmabuff, (void*)&U2TXREG, sizeof(dmabuff), 1, 1);

	DmaChnEnable(dmaTxChn);	// enable the DMA channel
}


/**
* Description: This function sends the frame to the uart
* Arguments  : none
* Returns    : none
*/
void UART_Transmit_SendFrame(byte *datatosend, byte size)
{
	//DmaChnDisable(dmaTxChn);	// disable the DMA channel
	memcpy(&dmabuff[2], datatosend, size);
	dmabuff[0] = UART_TX_FRAME_ID;
	dmabuff[1] = size;
	dmabuff[size + FRAME_HEADER_SIZE] = size ^ 0xFF;
	DmaChnSetTxfer(dmaTxChn, dmabuff, (void*)&U2TXREG, size + FRAME_HEADER_SIZE + 1, 1, 1);
	//DmaChnEnable(dmaTxChn);	// enable the DMA channel
	DmaTxferRes  res = DmaChnStartTxfer(dmaTxChn, DMA_WAIT_BLOCK, 1);
//	UART_Transmit_InitialiseFrame();
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
	byte offset = sample->offsetInFrame;
	byte resolutionAsByte = sample->res%8==0? sample->res/8 : sample->res/8 + 1;

	if(resolutionAsByte == 2)
	{
		toAdd = (sample->value & 0xff00) >> 8;
		dmabuff[offset++] = toAdd; 		
	}

	toAdd = sample->value & 0x00ff;
	dmabuff[offset++] = toAdd;
}


/**
* Description: This function adds a sample into the frame
* Arguments  : sample
* Returns    : none
*/
void UART_Transmit_InitialiseFrame()
{
	int i = 0;
	for(i = 0; i<TRANSMIT_BUFFER_SIZE; i++)
	{
		dmabuff[i] = 0;
	}

	dmabuff[OFFSET_TRAME_ID] = UART_TX_FRAME_ID;
	dmabuff[OFFSET_DATA_SIZE] = UART_TX_FRAME_DATA_SIZE;
	dmabuff[OFFSET_TX_CHECK_VALID] = UART_TX_FRAME_CHECK_VALID;
}
