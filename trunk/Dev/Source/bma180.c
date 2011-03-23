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
* File : bma180.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include "bma180.h"
#include <plib.h>
#include "global.h"
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


/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/
/**
 * @brief: Init the BMA180
 * @param: void
 * @return: void
*/
void BMA180_init(void)
{
	short byte;

	// UNLOCK WRITE
	byte = SPI_read(CTRL_REG0_ADDR);
    byte |= EE_W;                           // Have to set ee_w to	
    SPI_writeReg(CTRL_REG0_ADDR, byte);              
	//DelayMs(10);
	byte = SPI_read(CTRL_REG0_ADDR);             
	//DBPRINTF("\r\nREG0 = %x", byte);
	
	// DISABLE I2C
	byte = SPI_read(HIGH_DUR_ADDR);             
    byte |= DIS_I2C_MASK;                       
    SPI_writeReg(HIGH_DUR_ADDR, byte);          
	//DelayMs(10);
	byte = SPI_read(HIGH_DUR_ADDR);             
	//DBPRINTF("\r\n0xHIGH_DUR_ADDR = %x", byte);
	
	// DISABLE LOW & HIGH INT
	byte = SPI_read(HIGH_LOW_INFO_ADDR);             
    byte &= ~0xFF;                           // disable all
    SPI_writeReg(HIGH_LOW_INFO_ADDR, byte);          
	//DelayMs(10);
	byte = SPI_read(HIGH_LOW_INFO_ADDR);             
	//DBPRINTF("\r\n0xHIGH_LOW_INFO_ADDR = %x", byte);	
	
	// DISABLE SLOPE & TAPSENS INT
	byte = SPI_read(SLOPE_TAPSENS_INFO_ADDR);             
    byte &= ~0xFF;                           // Disable All
    SPI_writeReg(SLOPE_TAPSENS_INFO_ADDR, byte);           
	//DelayMs(10);
	byte = SPI_read(SLOPE_TAPSENS_INFO_ADDR);             
	//DBPRINTF("\r\nSLOPE_TAPSENS_INFO_ADDR = %x", byte);
	
	// ENABLE NEW DATA INT & DISABLE ADV INT
	byte = SPI_read(CTRL_REG3_ADDR);             
    byte = (byte & (~0x04)) | 0x02;                           // Disable All
    SPI_writeReg(CTRL_REG3_ADDR, byte);           
	//DelayMs(10);
	byte = SPI_read(CTRL_REG3_ADDR);             
	//DBPRINTF("\r\nCTRL_REG3_ADDR = %x", byte);
}

/**
 * @brief: Reset the BMA180
 * @param: void
 * @return: void
*/
void BMA180_softReset(void)
{
    SPI_writeReg(RESET_ADDR, RESET_VALUE);             // page 48
    //DelayMs(10);                        // wait 10 ms, see page 49
}

/**
 * @brief: Get sample axe X
 * @param: void
 * @return: short sample
*/
short Accel_getX()
{
	return Accel_get(ACCELERATION_X);
//	DBPRINTF("Acceleration X = %d\n", ret);	
}

/**
 * @brief: Get sample axe Y
 * @param: void
 * @return: short sample
 */
short Accel_getY()
{
	return Accel_get(ACCELERATION_Y);
//	DBPRINTF("Acceleration Y = %d\n", ret);
}

/**
 * @brief: Get sample axe Z
 * @param: void
 * @return: short sample
 */
short Accel_getZ()
{
	return Accel_get(ACCELERATION_Z);
//	DBPRINTF("Acceleration Z = %d\n\n", ret);
}

/**
 * @brief: Get sample axe x
 * @param: TAxe select the axe
 * @return: short sample
 */
short Accel_get(TAxe axe)
{
	
	unsigned char rx_lsb, rx_msb, txDataLsb, txDataMsb;
	unsigned short temp;
	short sample;

	switch(axe){
		case ACCELERATION_X:
			txDataLsb=ACCXLSB_ADDR;
			txDataMsb=ACCXMSB_ADDR;
			break;
		case ACCELERATION_Y:
			txDataLsb=ACCYLSB_ADDR;
			txDataMsb=ACCYMSB_ADDR;
			break;
		case ACCELERATION_Z:
			txDataLsb=ACCZLSB_ADDR;
			txDataMsb=ACCZMSB_ADDR;
			break;
		default:
			DBPRINTF("No data to received \n");
		return;
	}	
	rx_lsb = SPI_read(txDataLsb);
	rx_msb = SPI_read(txDataMsb);

	temp = (((rx_msb << 8) | (rx_lsb))>>2)&0x3FFF;
	if(temp >= 0x2000)
		sample = temp | 0xC000;
	else
		sample = temp;

	//DBPRINTF("\r\naccel = %f\tresult = 0x%x\t lsb = 0x%x\t msb = 0x%x", sample*0.00025, sample, rx_lsb, rx_msb);
	
	return sample;
}
