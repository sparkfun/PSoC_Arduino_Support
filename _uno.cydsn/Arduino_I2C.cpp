/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/


extern "C" {
#include <Project.h>
}

#include "stdint.h"
#include "Arduino_I2C.h"

uint8_t i2cRXBuffer[32];
uint8_t i2cTXBuffer[32];
uint8_t rxBufferSize;
uint8_t txBufferSize;
uint8_t slaveAddress;
uint8_t txOverflow;
uint8_t rxOverflow;

void I2C_begin()
{
    I2C_Start();
    A4_BYP |= A4_MASK;
    A5_BYP |= A5_MASK;
    A4_SetDriveMode(A4_DM_OD_LO);
    A5_SetDriveMode(A5_DM_OD_LO);
}

uint8_t I2C_requestFrom(uint8_t address, uint8_t quantity, \
    enum XFER_MODE stop)
{
    I2C_MasterClearStatus();
    
    if (stop == COMPLETE)
    {
        I2C_MasterReadBuf(address, i2cRXBuffer, quantity, \
            I2C_MODE_COMPLETE_XFER);
    }
    else if (stop == REPEATED_START)
    {
        I2C_MasterReadBuf(address, i2cRXBuffer, quantity, \
            I2C_MODE_REPEAT_START);
    }
    else
    {
        I2C_MasterReadBuf(address, i2cRXBuffer, quantity, I2C_MODE_NO_STOP);
    }
    
    while(0u == (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT));
    rxBufferSize = quantity;
    return quantity;
}

void I2C_beginTransmission(uint8_t address)
{
    slaveAddress = address;
}

uint8_t I2C_endTransmission(enum XFER_MODE stop)
{
    I2C_MasterClearStatus();
    
    if (stop == COMPLETE)
    {
        I2C_MasterWriteBuf(slaveAddress, i2cTXBuffer, txBufferSize, \
            I2C_MODE_COMPLETE_XFER);
    }
    else if (stop == REPEATED_START)
    {
        I2C_MasterWriteBuf(slaveAddress, i2cTXBuffer, txBufferSize, \
            I2C_MODE_REPEAT_START);
    }
    else
    {
        I2C_MasterWriteBuf(slaveAddress, i2cTXBuffer, txBufferSize, \
            I2C_MODE_NO_STOP);
    }
    
    while(0u == (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT));
    txBufferSize = 0;
    return 0;
}
uint8_t I2C_write(uint8_t *buffer, uint8_t length)
{
    uint8_t i;
    if (txBufferSize == 32)
    {
        txOverflow = 1;
        return 0;
    }
    for (i=0; i < length; i++)
    {
        i2cTXBuffer[txBufferSize+i] = buffer[i];
        if (++txBufferSize == 32)
        {
            txOverflow = 1;
            return i+1;
        }
    }
    return i;
}
uint8_t I2C_available()
{
    return txBufferSize;
}

uint8_t I2C_read()
{
    if (rxBufferSize == 0) return 0;
    return i2cRXBuffer[--rxBufferSize];
}

/* [] END OF FILE */
