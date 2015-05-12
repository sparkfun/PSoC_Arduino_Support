/*******************************************************************************
* File Name: Bootloader.c
* Version 1.30
*
*  Description:
*   Provides an API for the Bootloader component. The API includes functions
*   for starting boot loading operations, validating the application and
*   jumping to the application.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Bootloader_PVT.h"

#include "project.h"
#include <string.h>


/*******************************************************************************
* The Checksum and SizeBytes are forcefully set in code. We then post process
* the hex file from the linker and inject their values then. When the hex file
* is loaded onto the device these two variables should have valid values.
* Because the compiler can do optimizations to remove the constant
* accesses, these should not be accessed directly. Instead, the variables
* CyBtldr_ChecksumAccess & CyBtldr_SizeBytesAccess should be used to get the
* proper values at runtime.
*******************************************************************************/
#if defined(__ARMCC_VERSION) || defined (__GNUC__)
    __attribute__((section (".bootloader"), used))
#elif defined (__ICCARM__)
    #pragma location=".bootloader"
#endif  /* defined(__ARMCC_VERSION) || defined (__GNUC__) */

#if defined(__ARMCC_VERSION) || defined (__GNUC__) || defined (__C51__)
           const uint8  CYCODE Bootloader_Checksum = 0u;
#elif defined (__ICCARM__)
    __root const uint8  CYCODE Bootloader_Checksum = 0u;
#endif  /* defined(__ARMCC_VERSION) || defined (__GNUC__) || defined (__C51__) */
const uint8  CYCODE *Bootloader_ChecksumAccess  = (const uint8  CYCODE *)(&Bootloader_Checksum);

#if defined(__ARMCC_VERSION) || defined (__GNUC__)
    __attribute__((section (".bootloader"), used))
#elif defined (__ICCARM__)
    #pragma location=".bootloader"
#endif  /* defined(__ARMCC_VERSION) || defined (__GNUC__) */

const uint32 CYCODE Bootloader_SizeBytes = 0xFFFFFFFFu;
const uint32 CYCODE *Bootloader_SizeBytesAccess = (const uint32 CYCODE *)(&Bootloader_SizeBytes);

/***************************************
*     Function Prototypes
***************************************/
static void     Bootloader_HostLink(uint32 timeout);
static void     Bootloader_LaunchApplication(void) CYSMALL;
//static void     Bootloader_LaunchBootloadable(uint32 appAddr);
static char     BlockLoad(char mem, uint16 size, uint8* buffer, uint32 address);

/*******************************************************************************
* Function Name: Bootloader_Start
********************************************************************************
* Summary:
*  This function is called in order to execute the following algorithm:
*
*  - Identify the active bootloadable application (applicable only to
*    the Multi-application bootloader)
*
*  - Validate the bootloader application (design-time configurable, Bootloader
*    application validation option of the component customizer)
*
*  - Validate the active bootloadable application. If active bootloadable
*    application is not valid, and the other bootloadable application (inactive)
*    is valid, the last one is started.
*
*  - Run a communication subroutine (design-time configurable, Wait for command
*    option of the component customizer)
*
*  - Schedule the bootloadable and reset the device
*
* Parameters:
*  None
*
* Return:
*  This method will never return. It will either load a new application and
*  reset the device or jump directly to the existing application. The CPU is
*  halted, if validation failed when "Bootloader application validation" option
*  is enabled.
*  PSoC 3/PSoC 5: The CPU is halted if Flash initialization fails.
*
* Side Effects:
*  If Bootloader application validation option is enabled and this method
*  determines that the bootloader application itself is corrupt, this method
*  will not return, instead it will simply hang the application.
*
*******************************************************************************/
void Bootloader_Start(uint32 timeout) CYSMALL 
{

  if (CYRET_SUCCESS != CySetTemp())
  {
      CyHalt(0x00u);
  }
      
  if (Bootloader_GET_RUN_TYPE == Bootloader_START_BTLDR)
  {
      Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDR);
      Bootloader_HostLink(timeout);
  }

  /* Schedule bootloadable application and perform software reset */
  Bootloader_LaunchApplication();
}


/*******************************************************************************
* Function Name: Bootloader_LaunchApplication
********************************************************************************
*
* Summary:
*  Schedules bootloadable application and resets device
*
* Parameters:
*  None
*
* Returns:
*  This method will never return.
*
*******************************************************************************/
static void Bootloader_LaunchApplication(void) CYSMALL 
{    
    Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
    CySoftwareReset();
}

/*******************************************************************************
* Function Name: CyBtldr_CheckLaunch
********************************************************************************
*
* Summary:
*  This routine checks if the bootloader or the bootloadable application has to
*  be run.  If the application has to be run, it will start executing.
*  If the bootloader is to be run, it will return, so the bootloader can
*  continue starting up.
*
* Parameters:
*  None
*
* Returns:
*  It will not return if it determines that the bootloadable application should
*  be run.
*
*******************************************************************************/
void CyBtldr_CheckLaunch(void) CYSMALL 
{  

}
/* This doesn't need to be here, in this incarnation. Only needed if the 
CheckLaunch function is used to check conditions before main() begins.
__attribute__((noinline)) // Workaround for GCC toolchain bug with inlining
__attribute__((naked))
static void Bootloader_LaunchBootloadable(uint32 appAddr)
{
    __asm volatile("    BX  R0\n");
}
*/
/*******************************************************************************
* Function Name: Bootloader_HostLink
********************************************************************************
*
* Summary:
*  Causes the bootloader to attempt to read data being transmitted by the
*  host application.  If data is sent from the host, this establishes the
*  communication interface to process all requests.
*
* Parameters:
*  timeOut:
*   The amount of time to listen for data before giving up. Timeout is
*   measured in 10s of ms.  Use 0 for an infinite wait.
*
* Return:
*   None
*
*******************************************************************************/
static void Bootloader_HostLink(uint32 timeout) 
{
  uint16    CYDATA numberRead;
  cystatus  CYDATA readStat;
  uint16    bytesToRead;
  uint16    bytesToWrite;  
  uint32    counterVal;
  
  static const uint8 deviceID[7] = {'A','V','R','B', 'O', 'O', 'T'};
  static const uint8 swID[2] = {'1', '0'};
  static const uint8 hwID[2] = {'1', '0'};
  
  uint32 currentAddress = 0;

  uint8     packetBuffer[Bootloader_SIZEOF_COMMAND_BUFFER];

  /* Initialize communications channel. */
  CyBtldrCommStart();
  counterVal = 0;
  do
  {
    do
    {
      readStat = CyBtldrCommRead(packetBuffer,
                                 Bootloader_SIZEOF_COMMAND_BUFFER,
                                 &numberRead,
                                 10);
      counterVal = Reset_Timer_ReadCounter() * -1;
      #ifdef USE_UART
      char dbstring[24];
      sprintf(dbstring,"%ld\r", counterVal);
      UART_PutString(dbstring);
      CyDelay(50);
      #endif
      if (counterVal >= timeout)
      {    
        Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
        CySoftwareReset();
      }
    } while ( readStat != CYRET_SUCCESS );

    Reset_Timer_Stop();
    Reset_Timer_WriteCounter(0);
    Reset_Timer_Start();
    
    switch(packetBuffer[0])
    {
    /*************************************************************************
    *   Set read/write address
    *************************************************************************/
    case 'A':
      currentAddress = packetBuffer[1] << 9;
      currentAddress |= packetBuffer[2] << 1;
      packetBuffer[0] = '\r';
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;
      
    /*************************************************************************
    *   Set read/write address
    *************************************************************************/
    case 'H':
      currentAddress = packetBuffer[1] << 17;
      currentAddress |= packetBuffer[2] << 9;
      currentAddress |= packetBuffer[3] << 1;
      packetBuffer[0] = '\r';
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;
      
    /*************************************************************************
    *   Erase chip (unimplemented)
    *************************************************************************/
    case 'e':
      break;

    /*************************************************************************
    *   Enter/Leave bootloader mode - UNUSED
    *************************************************************************/
    case 'P':
    case 'L':
      packetBuffer[0] = '\r';
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;

    /*************************************************************************
    *   Exit bootloader
    *************************************************************************/
    case 'E':
      /* Normally, the CyBootloader checks the validity of the app here. We will
      *   assume that the app is valid b/c it was checked as it was being
      *   uploaded. */
      Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
      packetBuffer[0] = '\r';
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      while (USBUART_CDCIsReady() == 0)
      { /* wait for USB to finish sending response to the exit command */ }
      CySoftwareReset();
      /* Will never get here */
      break;
      
    /*************************************************************************
    *   Block read
    *************************************************************************/
    case 'g':
      bytesToRead = packetBuffer[1] << 8;
      bytesToRead |= packetBuffer[2];
      int16 idx;
      for(idx = 0u; idx < bytesToRead; idx++)
      {
        packetBuffer[idx] = Bootloader_GET_CODE_BYTE(currentAddress + idx);
      }
      CyBtldrCommWrite(packetBuffer, bytesToRead, NULL, 0);
      break;
      
    /*************************************************************************
    *   Block load
    *************************************************************************/  
    case 'B':
      bytesToWrite = packetBuffer[1]<<8 | packetBuffer[2];
      packetBuffer[0] = BlockLoad(packetBuffer[3], bytesToWrite, \
                                  packetBuffer + 4, currentAddress);
      if ((packetBuffer[0] == '\r') && packetBuffer[3] == 'F')
      {
        currentAddress += bytesToWrite;
      }
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;
    /*************************************************************************
    *   Report device ID
    *************************************************************************/
    case 'S':
      CyBtldrCommWrite((uint8*)deviceID, 8, NULL, 0);
      break;
    /*************************************************************************
    *   Report firmware revision
    *************************************************************************/
    case 'V':
      CyBtldrCommWrite((uint8*)swID, 2, NULL, 0);
      break;
    /*************************************************************************
    *   Report programmer type ('S' for "Serial")
    *************************************************************************/
    case 'p':
      packetBuffer[0] = 'S';
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;
    /*************************************************************************
    *   Report autoincrement address support ('Y' for "Yes")
    *************************************************************************/
    case 'a':
      packetBuffer[0] = 'Y';
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;
    /*************************************************************************
    *   Report block write support ('Y' for "Yes", then two bytes of block size
    *    written MSB first.
    *************************************************************************/
    case 'b':
      packetBuffer[0] = 'Y';
      packetBuffer[1] = 0x01;
      packetBuffer[2] = 0x00;
      CyBtldrCommWrite(packetBuffer, 3, NULL, 0);
      break;
    /*************************************************************************
    *   Report hardware device version
    *************************************************************************/
    case 'v':
      CyBtldrCommWrite((uint8*)hwID, 2, NULL, 0);
      break;
    /*************************************************************************
    *   Report (bogus) part ID, followed by list terminator 0x00.
    *************************************************************************/
    case 't':
      packetBuffer[0] = 0x44;
      packetBuffer[1] = 0;
      CyBtldrCommWrite(packetBuffer, 2, NULL, 0);
      break;
    /*************************************************************************
    *   Report device signature (this is that of the Atmega32u4)
    *************************************************************************/
    case 's':
      packetBuffer[0] = 0x87;
      packetBuffer[1] = 0x95;
      packetBuffer[2] = 0x1e;
      CyBtldrCommWrite(packetBuffer, 3, NULL, 0);
      break;
    /*************************************************************************
    *   Unimplemented fuse AVR fuse register read/write
    *************************************************************************/
    case 'N':  
    case 'Q':
    case 'F':
    case 'r':
      packetBuffer[0] = 0x00;
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;
    /*************************************************************************
    *   Unused but implemented in AVRDUDE, so some response needed.
    *************************************************************************/
    case 'T':
    case 'x':
    case 'y':
      packetBuffer[0] = '\r';
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;
    /*************************************************************************
    *   Unsupported command
    *************************************************************************/
    default:
      packetBuffer[0] = '?';
      CyBtldrCommWrite(packetBuffer, 1, NULL, 0);
      break;
    }
  } while (1);
}

/*******************************************************************************
* Function Name: Bootloader_SetFlashByte
********************************************************************************
*
* Summary:
*  Writes a byte to the specified Flash memory location.
*
* Parameters:
*  address:
*      The address in Flash memory where data will be written
*
*  runType:
*      Byte to be written
*
* Return:
*  None
*
*******************************************************************************/
void Bootloader_SetFlashByte(uint32 address, uint8 runType) 
{
  uint32 flsAddr = address - CYDEV_FLASH_BASE;
  uint8  rowData[CYDEV_FLS_ROW_SIZE];

  uint8 arrayId = ( uint8 )(flsAddr / CYDEV_FLS_SECTOR_SIZE);
  uint16 rowNum = ( uint16 )((flsAddr % CYDEV_FLS_SECTOR_SIZE) / CYDEV_FLS_ROW_SIZE);

  uint32 baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
  uint16 idx;

  for(idx = 0u; idx < CYDEV_FLS_ROW_SIZE; idx++)
  {
      rowData[idx] = Bootloader_GET_CODE_BYTE(baseAddr + idx);
  }

  rowData[address % CYDEV_FLS_ROW_SIZE] = runType;

  (void) CyWriteRowData(arrayId, rowNum, rowData);

  /***************************************************************************
  * When writing Flash, data in the instruction cache can become stale.
  * Therefore, the cache data does not correlate to the data just written to
  * Flash. A call to CyFlushCache() is required to invalidate the data in the
  * cache and force fresh information to be loaded from Flash.
  ***************************************************************************/
  CyFlushCache();
}

/*******************************************************************************
* Function Name: BlockLoad
********************************************************************************
*
* Summary:
*  Writes an arbitrary block to NVM (flash or EEPROM)
*
* Parameters:
*   char   mem: 'E' or 'F' for EEPROM or Flash; others undefined
*   uint16 size: number of bytes to be written
*   uint8* buffer: pointer to buffer to be written
*
* Return:
*  The result of the write attempt.
*   '\r' - okay
*   '?'  - failure for some reason
*
*******************************************************************************/
static char BlockLoad(char mem, uint16 size, uint8* buffer, uint32 address)
{
  char retVal = '?';
  if ((mem != 'E') && (mem != 'F'))
  {
    return retVal;
  }
  
  /* Passed the first test: this is a valid write. Now, which kind? */
  if (mem == 'E')
  {
    /* EEPROM writes unimplmented (yet) */
  }
  else if (mem == 'F')
  {
    uint8  rowData[CYDEV_FLS_ROW_SIZE];
    uint8* dataBuffer = buffer;
    
    /* There are several pieces of information we may need:
    *   arrayId - which *array* of flash is this? This is a big block of many
    *             rows; in the 5888, it's 0x10000 long.
    *   rowNum  - which row within that array? *Not* absolute row id!
    *   baseAddr - if the address is not row aligned, which row does it start
    *             within?
    *   offset  - where, within that row, does the data start?
    *   multirow - do we need to do more than one row write to cover this data?
    */
    
    uint8 arrayId = ( uint8 )(address / CYDEV_FLS_SECTOR_SIZE);
    uint16 rowNum = ( uint16 )((address % CYDEV_FLS_SECTOR_SIZE) / \
                                CYDEV_FLS_ROW_SIZE);

    uint32 baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
    uint32 offset = address % CYDEV_FLS_ROW_SIZE;
    uint8 multiRow = 0;
    uint8 currentRow = 0;
    if (size > (CYDEV_FLS_ROW_SIZE - offset))
    {
      multiRow = 1;
    }
    
    /* Metadata created; let's write! */
    do
    {
      uint16 idx;
      /* Partial row write section */
      if ((size != CYDEV_FLS_ROW_SIZE) || (offset != 0))
      {
        /* If this isn't a full row write, we need to cache the existing row */
        for(idx = 0u; idx < CYDEV_FLS_ROW_SIZE; idx++)
        {
          rowData[idx] = Bootloader_GET_CODE_BYTE(baseAddr + \
                         (CYDEV_FLS_ROW_SIZE*currentRow) + idx);
        }
        dataBuffer = rowData;
        /* Now load the passed data into the dataBuffer at the offset, making
        *   sure that we don't write past the end of the buffer */
        memcpy(dataBuffer + offset, buffer, \
          ((CYDEV_FLS_ROW_SIZE - offset) > size) ? \
          (CYDEV_FLS_ROW_SIZE - offset) : \
          size);
      }
      (void) CyWriteRowData(arrayId, rowNum, dataBuffer);
      buffer += (CYDEV_FLS_ROW_SIZE - offset);
      offset = 0;
      ++currentRow;
    }  while (currentRow <= multiRow);
    retVal = '\r';
    /*************************************************************************
    * When writing Flash, data in the instruction cache can become stale.
    * Therefore, the cache data does not correlate to the data just written to
    * Flash. A call to CyFlushCache() is required to invalidate the data in 
    * the cache and force fresh information to be loaded from Flash.
    *************************************************************************/
    CyFlushCache();
  }
  return retVal;
}

/* [] END OF FILE */
