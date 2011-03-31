
#ifndef _I2C_H
#define _I2C_H

BOOL StartTransfer( BOOL restart );
BOOL TransmitOneByte( UINT8 data );
void I2C_Init(unsigned int i2c_clock);
void StopTransfer( void );
BOOL I2C_WriteByte(unsigned char myslave, unsigned char RegAddress, unsigned char Data);
unsigned char I2C_ReadByte(unsigned char myslave, unsigned char RegAddress);

#endif // _I2C_H
