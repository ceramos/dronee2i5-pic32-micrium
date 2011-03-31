#include <plib.h>
#include "itg3200.h"
#include "i2c.h"
#include "bsp.h"
#ifdef PIC32_STARTER_KIT
    #include <stdio.h>
#endif

long cpt_error_i2c = 0;
/*******************************************************************************
  Function:
    BOOL StartTransfer( BOOL restart )

  Summary:
    Starts (or restarts) a transfer to/from the EEPROM.

  Description:
    This routine starts (or restarts) a transfer to/from the EEPROM, waiting (in
    a blocking loop) until the start (or re-start) condition has completed.

  Precondition:
    The I2C module must have been initialized.

  Parameters:
    restart - If FALSE, send a "Start" condition
            - If TRUE, send a "Restart" condition
    
  Returns:
    TRUE    - If successful
    FALSE   - If a collision occured during Start signaling
    
  Example:
    <code>
    StartTransfer(FALSE);
    </code>

  Remarks:
    This is a blocking routine that waits for the bus to be idle and the Start
    (or Restart) signal to complete.
  *****************************************************************************/
void I2C_Init(unsigned int i2c_clock)
{
	UINT32              actualClock;
	// Set the I2C baudrate

	actualClock = I2CSetFrequency(ACCEL_I2C_BUS, GetPeripheralClock(), i2c_clock);
    if ( abs(actualClock-I2C_CLOCK_FREQ) > I2C_CLOCK_FREQ/10 )
    {
        //DBPRINTF("Error: I2C1 clock frequency (%u) error exceeds 10%%.\n", (unsigned)actualClock);
    }

    // Enable the I2C bus
    I2CEnable(ACCEL_I2C_BUS, TRUE);	
}

/*******************************************************************************
  Function:
    BOOL StartTransfer( BOOL restart )

  Summary:
    Starts (or restarts) a transfer to/from the EEPROM.

  Description:
    This routine starts (or restarts) a transfer to/from the EEPROM, waiting (in
    a blocking loop) until the start (or re-start) condition has completed.

  Precondition:
    The I2C module must have been initialized.

  Parameters:
    restart - If FALSE, send a "Start" condition
            - If TRUE, send a "Restart" condition
    
  Returns:
    TRUE    - If successful
    FALSE   - If a collision occured during Start signaling
    
  Example:
    <code>
    StartTransfer(FALSE);
    </code>

  Remarks:
    This is a blocking routine that waits for the bus to be idle and the Start
    (or Restart) signal to complete.
  *****************************************************************************/
BOOL StartTransfer( BOOL restart )
{
    I2C_STATUS  status;

    // Send the Start (or Restart) signal
    if(restart)
    {
        I2CRepeatStart(ACCEL_I2C_BUS);
    }
    else
    {
        // Wait for the bus to be idle, then start the transfer
        while( !I2CBusIsIdle(ACCEL_I2C_BUS) );

        if(I2CStart(ACCEL_I2C_BUS) != I2C_SUCCESS)
        {
            //DBPRINTF("Error: Bus collision during transfer Start\n");
            return FALSE;
        }
    }

    // Wait for the signal to complete
    do
    {
        status = I2CGetStatus(ACCEL_I2C_BUS);

    } while ( !(status & I2C_START) );

    return TRUE;
}


/*******************************************************************************
  Function:
    BOOL TransmitOneByte( UINT8 data )

  Summary:
    This transmits one byte to the EEPROM.

  Description:
    This transmits one byte to the EEPROM, and reports errors for any bus
    collisions.

  Precondition:
    The transfer must have been previously started.

  Parameters:
    data    - Data byte to transmit

  Returns:
    TRUE    - Data was sent successfully
    FALSE   - A bus collision occured

  Example:
    <code>
    TransmitOneByte(0xAA);
    </code>

  Remarks:
    This is a blocking routine that waits for the transmission to complete.
  *****************************************************************************/

BOOL TransmitOneByte( UINT8 data )
{
    // Wait for the transmitter to be ready
    while(!I2CTransmitterIsReady(ACCEL_I2C_BUS));

    // Transmit the byte
    if(I2CSendByte(ACCEL_I2C_BUS, data) == I2C_MASTER_BUS_COLLISION)
    {
        //DBPRINTF("Error: I2C Master Bus Collision\n");
        return FALSE;
    }

    // Wait for the transmission to finish
    while(!I2CTransmissionHasCompleted(ACCEL_I2C_BUS));

    return TRUE;
}


/*******************************************************************************
  Function:
    void StopTransfer( void )

  Summary:
    Stops a transfer to/from the EEPROM.

  Description:
    This routine Stops a transfer to/from the EEPROM, waiting (in a 
    blocking loop) until the Stop condition has completed.

  Precondition:
    The I2C module must have been initialized & a transfer started.

  Parameters:
    None.
    
  Returns:
    None.
    
  Example:
    <code>
    StopTransfer();
    </code>

  Remarks:
    This is a blocking routine that waits for the Stop signal to complete.
  *****************************************************************************/

void StopTransfer( void )
{
    I2C_STATUS  status;

    // Send the Stop signal
    I2CStop(ACCEL_I2C_BUS);

    // Wait for the signal to complete
    do
    {
        status = I2CGetStatus(ACCEL_I2C_BUS);

    } while ( !(status & I2C_STOP) );
}

/*******************************************************************************
  Function:
    void StopTransfer( void )

  Summary:
    Stops a transfer to/from the EEPROM.

  Description:
    This routine Stops a transfer to/from the EEPROM, waiting (in a 
    blocking loop) until the Stop condition has completed.

  Precondition:
    The I2C module must have been initialized & a transfer started.

  Parameters:
    None.
    
  Returns:
    None.
    
  Example:
    <code>
    StopTransfer();
    </code>

  Remarks:
    This is a blocking routine that waits for the Stop signal to complete.
  *****************************************************************************/
BOOL I2C_WriteByte(unsigned char myslave, unsigned char RegAddress, unsigned char Data)
{
	unsigned char Index, i2cData[3], DataSz;	//2= Slave address + Register address
	I2C_7_BIT_ADDRESS   SlaveAddress;
	BOOL                Acknowledged, Success = TRUE;
	
	// Initialize the data buffer
    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, myslave, I2C_WRITE);
    i2cData[0] = SlaveAddress.byte;
    i2cData[1] = RegAddress;              // ITG3200 Register
    i2cData[2] = Data; 
    DataSz = 3;
    
	// Start the transfer to write data to the EEPROM
    if( !StartTransfer(FALSE) )
    {
        while(1);
    }

    // Transmit all data
    Index = 0;
    while( Success && (Index < DataSz) )
    {
        // Transmit a byte
        if (TransmitOneByte(i2cData[Index]))
        {
            // Advance to the next byte
            Index++;

            // Verify that the byte was acknowledged
            if(!I2CByteWasAcknowledged(ACCEL_I2C_BUS))
            {
                //DBPRINTF("Error: Sent byte was not acknowledged\n");
                Success = FALSE;
            }
        }
        else
        {
            Success = FALSE;
        }
    }
	
    // End the transfer (hang here if an error occured)
    StopTransfer();
    if(!Success)
    {
        while(1);
    }

    // Wait for EEPROM to complete write process, by polling the ack status.
    Acknowledged = FALSE;
    do
    {
        // Start the transfer to address the EEPROM
        if( !StartTransfer(FALSE) )
        {
            while(1);
        }
        
        // Transmit just the EEPROM's address
        if (TransmitOneByte(SlaveAddress.byte))
        {
            // Check to see if the byte was acknowledged
            Acknowledged = I2CByteWasAcknowledged(ACCEL_I2C_BUS);
        }
        else
        {
            Success = FALSE;
        }

        // End the transfer (stop here if an error occured)
        StopTransfer();
        if(!Success)
        {
            while(1);
        }

    } while (Acknowledged != TRUE);
    return Success;
}

/*******************************************************************************
  Function:
    void StopTransfer( void )

  Summary:
    Stops a transfer to/from the EEPROM.

  Description:
    This routine Stops a transfer to/from the EEPROM, waiting (in a 
    blocking loop) until the Stop condition has completed.

  Precondition:
    The I2C module must have been initialized & a transfer started.

  Parameters:
    None.
    
  Returns:
    None.
    
  Example:
    <code>
    StopTransfer();
    </code>

  Remarks:
    This is a blocking routine that waits for the Stop signal to complete.
  *****************************************************************************/
unsigned char I2C_ReadByte(unsigned char myslave, unsigned char RegAddress)
{
    unsigned char Index, i2cData[3], DataSz;	//2= Slave address + Register address
	unsigned char dataread;
	I2C_7_BIT_ADDRESS   SlaveAddress;
	BOOL                Success = TRUE;
	
    // Initialize the data buffer
    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, myslave, I2C_WRITE);
    i2cData[0] = SlaveAddress.byte;
    i2cData[1] = RegAddress;              // EEPROM location to read (high address byte)
    DataSz = 2;
    
    // Start the transfer to read the EEPROM.
    if( !StartTransfer(FALSE) )
    {
        while(1);
    }
    
    // Address the EEPROM.
    Index = 0;
    while( Success & (Index < DataSz) )
    {
        // Transmit a byte
        if (TransmitOneByte(i2cData[Index]))
        {
            // Advance to the next byte
            Index++;
        }
        else
        {
            Success = FALSE;
        }

        // Verify that the byte was acknowledged
        if(!I2CByteWasAcknowledged(ACCEL_I2C_BUS))
        {
            //DBPRINTF("Error: Sent byte was not acknowledged\n");
            Success = FALSE;
        }
    }

    // Restart and send the EEPROM's internal address to switch to a read transfer
    if(Success)
    {
        // Send a Repeated Started condition
        if( !StartTransfer(TRUE) )
        {
            while(1);
        }

        // Transmit the address with the READ bit set
        I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, myslave, I2C_READ);
        if (TransmitOneByte(SlaveAddress.byte))
        {
            // Verify that the byte was acknowledged
            if(!I2CByteWasAcknowledged(ACCEL_I2C_BUS))
            {
                //DBPRINTF("Error: Sent byte was not acknowledged\n");
                Success = FALSE;
            }
        }
        else
        {
            Success = FALSE;
        }
    }

    // Read the data from the desired address
    if(Success)
    {
        if(I2CReceiverEnable(ACCEL_I2C_BUS, TRUE) == I2C_RECEIVE_OVERFLOW)
        {
            //DBPRINTF("Error: I2C Receive Overflow\n");
            Success = FALSE;
        }
        else
        {
            while(!I2CReceivedDataIsAvailable(ACCEL_I2C_BUS));
            dataread = I2CGetByte(ACCEL_I2C_BUS);
        }

    }

    // End the transfer (stop here if an error occured)
    StopTransfer();
    if(!Success)
    {
        //while(1);
        cpt_error_i2c++;
    }
	return dataread;
}
