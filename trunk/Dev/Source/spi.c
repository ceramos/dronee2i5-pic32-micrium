/*********************************************************************
 *
 *                  SPI API Example file
 *
 *********************************************************************
 * FileName:        loopback.c
 * Dependencies:	Spi.h
 *
 * Processor:       PIC32
 *
 * Complier:        MPLAB Cxx
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * $Id: spi_api_example.c 4172 2007-08-14 22:17:03Z aura $
 *
 ********************************************************************/
#include <stdlib.h>

#include <plib.h>

#include "global.h"
#include "spi.h"
#include "bma180.h"

static void delayus(int us);

/**
 * @brief: Initialisation SPI 
 * @param: 	void
 * @return: void
*/
void SPI_Init(void)
{
    // Turn off leds before configuring the IO pin as output
   	SET_CSB;            // same as LATDCLR = 0x0007
	mPORTBSetPinsDigitalOut(CSB_BIT);    // same as TRISDCLR = 0x0007


	unsigned int config1 = FRAME_ENABLE_OFF|ENABLE_SDO_PIN|SPI_MODE8_ON|SPI_SMP_OFF|SPI_CKE_OFF|SLAVE_ENABLE_OFF|CLK_POL_ACTIVE_LOW|MASTER_ENABLE_ON|SEC_PRESCAL_4_1|PRI_PRESCAL_16_1;
	unsigned int config2 = SPI_ENABLE|SPI_IDLE_CON;
	
	
	OpenSPI1(config1, config2);
//	DBPRINTF("\r\nSPI INIT FINISHED\n");

}

 /*********************************************************************
 * Function:        void Data_Send_SPI(int data_to_send)
 * PreCondition:    None
 * Input:           data_to_send : Données à envoyer à l'ADC
 * Output:          None
 * Side Effects:    None
 * Overview:        Envoi des données à l'ADC
 ********************************************************************/
 void Data_Send_SPI(unsigned char data_to_send)
{
	unsigned char Waste_Filled = 0;

	SPI1BUF = data_to_send;
	while (!SPI1STATbits.SPIRBF);
	//--- Lecture du buffer pour éviter les Overflow ---//
	Waste_Filled = SPI1BUF;
}

 /*********************************************************************
 * Function:        int Data_Received_SPI()
 * PreCondition:    None
 * Input:           None
 * Output:          Données reçues de l'ADC
 * Side Effects:    None
 * Overview:        Réception des données de l'ADC
 ********************************************************************/
unsigned char Data_Received_SPI(void)
{		
	SPI1BUF = 0x00;
	
	//--- Check for Receive buffer full status bit of status register ---//
	while(!SPI1STATbits.SPIRBF);
	//--- RAZ Flag Overflow ---//
	SPI1STATbits.SPIROV = 0;	
	//--- Return Value ---//
	return  SPI1BUF;          
}

/**
 * @brief: Read on SPI
 * @param: unsigned char: address of the register to read
 * @return: unsigned char value of the regsiter
*/
unsigned char SPI_read(unsigned char address)
{
	//returns the contents of any 1 byte register from any address
	//sets the MSB for every address byte (READ mode)
	unsigned char byte;

	address |= 0x80;

	CLEAR_CSB;
	
	Data_Send_SPI(address);	// send data
	byte=Data_Received_SPI();//inutile=Data_Received_SPI();		//Ce byte ne sera pas lu car nul
	SET_CSB;
	return byte;
}

/**
 * @brief: Write on SPI 2 bytes
 * @param: unsigned char address, unsigned char data
 * @return: void
*/
void SPI_writeReg(unsigned char address, unsigned char data)
{
    address &= 0x7F;                        //Force a write (bit 7=0)
    
    CLEAR_CSB;                                   //Select SPI device
    Data_Send_SPI(address);                     //Send register location                                  //Select SPI device
	Data_Send_SPI(data);                        //Send value to record into register
    SET_CSB;
}
