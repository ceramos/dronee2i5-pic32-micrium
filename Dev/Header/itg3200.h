/*
    ITG3200.h
	April 28, 2010
	by: Jim Lindblom
*/

#ifndef _ITG3200_H
#define _ITG3200_H

#define I2C_CLOCK_FREQ              100000

// EEPROM Constants
#define ACCEL_I2C_BUS              I2C1
#define GYRO_ADDRESS              0x69        // 0b1010000 Serial EEPROM address
// ITG3200 Register Defines
#define WHO		0x00
#define	SMPL	0x15
#define DLPF	0x16
#define INT_C	0x17
#define INT_S	0x1A
#define	TMP_H	0x1B
#define	TMP_L	0x1C
#define	GX_H	0x1D
#define	GX_L	0x1E
#define	GY_H	0x1F
#define	GY_L	0x20
#define GZ_H	0x21
#define GZ_L	0x22
#define PWR_M	0x3E

void Gyro_Init(void);
short Gyro_ReadAxe(unsigned char Axe);
short Gyro_getX(void);
short Gyro_getY(void);
short Gyro_getZ(void);
char Gyro_WriteReg(unsigned char Regaddr, unsigned char Data);
unsigned char Gyro_ReadReg(unsigned char Regaddr);

#endif //_ITG3200_H
