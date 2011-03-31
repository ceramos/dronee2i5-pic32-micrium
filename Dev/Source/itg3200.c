
#include <plib.h>
#include "itg3200.h"
#include "debug.h"
#include "i2c.h"
#ifdef PIC32_STARTER_KIT
    #include <stdio.h>
#endif


void Gyro_Init(void)
{
	// Set the I2C baudrate & Enable the I2C bus
	I2C_Init(I2C_CLOCK_FREQ);	
}

short Gyro_ReadAxe(unsigned char Axe)
{	
	short dataread;
	
	// Read the data back from the Gyro.
    //
    dataread = I2C_ReadByte(GYRO_ADDRESS, Axe) << 8;
    dataread |= I2C_ReadByte(GYRO_ADDRESS, Axe+1) & 0x00FF;
    
	return dataread;
}

short Gyro_getX(void)
{
#ifdef DEBUG_GYRO_X_DEFAUT_VALUE
	return DEBUG_GYRO_X_DEFAUT_VALUE;
#else
	return Gyro_ReadAxe(GX_H);
#endif
}

short Gyro_getY(void)
{
#ifdef DEBUG_GYRO_X_DEFAUT_VALUE
	return DEBUG_GYRO_X_DEFAUT_VALUE;
#else
	return Gyro_ReadAxe(GY_H);
#endif
}

short Gyro_getZ(void)
{
#ifdef DEBUG_GYRO_X_DEFAUT_VALUE
	return DEBUG_GYRO_X_DEFAUT_VALUE;
#else
	return Gyro_ReadAxe(GZ_H);
#endif
}

char Gyro_WriteReg(unsigned char Regaddr, unsigned char Data)
{
	return I2C_WriteByte(GYRO_ADDRESS, Regaddr, Data);
}

unsigned char Gyro_ReadReg(unsigned char Regaddr)
{
	unsigned char data;
	
	// Read the data back from the Gyro.
    //
    data = I2C_ReadByte(GYRO_ADDRESS, Regaddr);
	return data;
}
