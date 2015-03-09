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

/*******************************************************************************
* Clarifications from SparkFun Feb 2015
*  To make things a little clearer, we've removed all the non-pertinent
*  functionality and IFDEF stuff that doesn't get included for us, since we know
*  what the target is and what our build environment is. We've also added and
*  changed some things; this is *not* the stock Bootloader.c file!!!
*******************************************************************************/


#include "Bootloader_PVT.h"
#include "Bootloader.h"

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

__attribute__((section (".bootloader"), used))
const uint8  CYCODE Bootloader_Checksum = 0u;
const uint8  CYCODE *Bootloader_ChecksumAccess  = (const uint8  CYCODE *)(&Bootloader_Checksum);
__attribute__((section (".bootloader"), used))

const uint32 CYCODE Bootloader_SizeBytes = 0xFFFFFFFFu;
const uint32 CYCODE *Bootloader_SizeBytesAccess = (const uint32 CYCODE *)(&Bootloader_SizeBytes);

#define Bootloader_activeApp      (Bootloader_MD_BTLDB_ACTIVE_0)

/***************************************
*     Function Prototypes
***************************************/
//static cystatus Bootloader_WritePacket(uint8 status, uint8 buffer[], uint16 size) CYSMALL;
//static uint16   Bootloader_CalcPacketChecksum(const uint8 buffer[], uint16 size) CYSMALL;
static void     Bootloader_LaunchBootloadable(uint32 appAddr);

/*******************************************************************************
* Function Name: Bootloader_CalcPacketChecksum
********************************************************************************
*
* Summary:
*  This computes the 16 bit checksum for the provided number of bytes contained
*  in the provided buffer
*
* Parameters:
*  buffer:
*     The buffer containing the data to compute the checksum for
*  size:
*     The number of bytes in the buffer to compute the checksum for
*
* Returns:
*  16 bit checksum for the provided data
*
*******************************************************************************/
#if 0
static uint16 Bootloader_CalcPacketChecksum(const uint8 buffer[], uint16 size) \
                    CYSMALL 
{
  uint16 CYDATA sum = 0u;

  while (size > 0u)
  {
    sum += buffer[size - 1u];
    size--;
  }

  return(( uint16 )1u + ( uint16 )(~sum));
}
#endif
/*******************************************************************************
* Function Name: Bootloader_Calc8BitSum
********************************************************************************
*
* Summary:
*  This computes the 8 bit sum for the provided number of bytes contained in
*  FLASH (if baseAddr equals CY_FLASH_BASE) or EEPROM (if baseAddr equals
*  CY_EEPROM_BASE).
*
* Parameters:
* baseAddr:
*   CY_FLASH_BASE
*   CY_EEPROM_BASE - applicable only for PSoC 3 / PSoC 5LP devices.
*
*  start:
*     The starting address to start summing data for
*  size:
*     The number of bytes to read and compute the sum for
*
* Returns:
*   8 bit sum for the provided data
*
*******************************************************************************/
uint8 Bootloader_Calc8BitSum(uint32 baseAddr, uint32 start, uint32 size) \
                CYSMALL 
{
    uint8 CYDATA sum = 0u;

    CYASSERT((baseAddr == CY_EEPROM_BASE) || (baseAddr == CY_FLASH_BASE));

    while (size > 0u)
    {
        size--;
        sum += (*((uint8  *)(baseAddr + start + size)));
    }

    return(sum);
}


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
void Bootloader_Start(cystatus* validApp) CYSMALL 
{
  *validApp  = CYRET_BAD_DATA;
  
  if (CYRET_SUCCESS == Bootloader_ValidateBootloadable(Bootloader_MD_BTLDB_ACTIVE_0))
  {
      *validApp = CYRET_SUCCESS;
  }

  /***********************************************************************
  * If the active bootloadable application is invalid or a bootloader
  * application is scheduled - do the following:
  *  - schedule the bootloader application to be run after software reset
  *  - Return to main, to wait for the bootloader to bootload.
  ***********************************************************************/
  if (*validApp == CYRET_SUCCESS )
  {
    CyBtldr_CheckLaunch();
    Bootloader_SET_RUN_TYPE(Bootloader_EXIT_TO_BTLDB);
  }
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
void Bootloader_LaunchApplication(void) CYSMALL 
{
    /* Schedule Bootloadable to start after reset */
    Bootloader_SET_RUN_TYPE(Bootloader_START_APP);

    CySoftwareReset();
}


/*******************************************************************************
* Function Name: Bootloader_Exit
********************************************************************************
*
* Summary:
*  Schedules the specified application and performs software reset to launch
*  a specified application.
*
*  If the specified application is not valid, the Bootloader (the result of the
*  ValidateBootloadable() function execution returns other than CYRET_SUCCESS,
*  the bootloader application is launched.
*
* Parameters:
*  appId: application to be started:
*  Bootloader_EXIT_TO_BTLDR   - Bootloader application will be started on
*                                     software reset.
*  Bootloader_EXIT_TO_BTLDB,
*  Bootloader_EXIT_TO_BTLDB_1 - Bootloadable application # 1 will be
*                                     started on software reset.
*  Bootloader_EXIT_TO_BTLDB_2 - Bootloadable application # 2 will be
*                                     started on software reset. Available only
*                                     if Multi-Application option is enabled in
*                                     the component customizer.
* Returns:
*  This function never returns.
*
*******************************************************************************/
void Bootloader_Exit(uint8 appId) CYSMALL 
{
  if(Bootloader_EXIT_TO_BTLDR == appId)
  {
      Bootloader_SET_RUN_TYPE(0x0u);
  }
  else
  {
    if(CYRET_SUCCESS == Bootloader_ValidateBootloadable(appId))
    {
      /* Set active application in metadata */
      uint8 CYDATA idx;
      for(idx = 0u; idx < Bootloader_MAX_NUM_OF_BTLDB; idx++)
      {
        Bootloader_SetFlashByte( \
            (uint32) Bootloader_MD_BTLDB_ACTIVE_OFFSET(idx), \
            (uint8 )(idx == appId));
      }

      Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
    }
    else
    {
        Bootloader_SET_RUN_TYPE(0u);
    }
  }

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
  if (Bootloader_GET_RUN_TYPE == Bootloader_START_APP)
  {
    Bootloader_SET_RUN_TYPE(0u);

    /*******************************************************************************
    * Indicates that we have told ourselves to jump to the application since we have
    * already told ourselves to jump, we do not do any expensive verification of the
    * application. We just check to make sure that the value at CY_APP_ADDR_ADDRESS
    * is something other than 0.
    *******************************************************************************/
    if(0u != Bootloader_GetMetadata(Bootloader_GET_BTLDB_ADDR, 
                                      Bootloader_activeApp))
    {
      /* Never return from this method */
      Bootloader_LaunchBootloadable(Bootloader_GetMetadata( \
                                    Bootloader_GET_BTLDB_ADDR, \
                                    Bootloader_activeApp));
    }
  }
}


    __attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
    __attribute__((naked))
    static void Bootloader_LaunchBootloadable(uint32 appAddr)
    {
        __asm volatile("    BX  R0\n");
    }



/*******************************************************************************
* Function Name: Bootloader_ValidateBootloadable
********************************************************************************
* Summary:
*  Performs the bootloadable application validation by calculating the
*  application image checksum and comparing it with the checksum value stored
*  in the Bootloadable Application Checksum field of the metadata section.
*
*  If the Fast bootloadable application validation option is enabled in the
*  component customizer and bootloadable application successfully passes
*  validation, the Bootloadable Application Verification Status field of the
*  metadata section is updated. Refer to the Metadata Layout section for the
*  details.
*
*  If the Fast bootloadable application validation option is enabled and
*  Bootloadable Application Verification Status field of the metadata section
*  claims that bootloadable application is valid, the function returns
*  CYRET_SUCCESS without further checksum calculation.
*
* Parameters:
*  appId:
*  The number of the bootloadable application should be 0 for the normal
*  bootloader and 0 or 1 for the Multi-Application bootloader.
*
* Returns:
*  Returns CYRET_SUCCESS if the specified bootloadable application is valid.
*
*******************************************************************************/
cystatus Bootloader_ValidateBootloadable(uint8 appId) CYSMALL \
{
  uint32 CYDATA idx;

  uint32 CYDATA end   = Bootloader_FIRST_APP_BYTE(appId) +
                          Bootloader_GetMetadata(Bootloader_GET_BTLDB_LENGTH,
                                                 appId);

  CYBIT valid = 0u; /* Assume bad flash image */
  uint8  CYDATA calcedChecksum = 0u;

  /* Calculate checksum of bootloadable image */
  for(idx = Bootloader_FIRST_APP_BYTE(appId); idx < end; ++idx)
  {
      uint8 CYDATA curByte = Bootloader_GET_CODE_BYTE(idx);

      if((curByte != 0u) && (curByte != 0xFFu))
      {
          valid = 1u;
      }

      calcedChecksum += curByte;
  }


  /***************************************************************************
  * We do not compute checksum over the meta data section, so no need to
  * subtract off App Verified or App Active information here like we do when
  * verifying a row.
  ***************************************************************************/

  /* Add ECC data to checksum */
  idx = ((Bootloader_FIRST_APP_BYTE(appId)) >> 3u);

  /* Flash may run into meta data, so ECC does not use full row */
  end = (end == (CY_FLASH_SIZE - Bootloader_MD_SIZEOF))
      ? (CY_FLASH_SIZE >> 3u)
      : (end >> 3u);

  for (; idx < end; ++idx)
  {
    calcedChecksum += CY_GET_XTND_REG8((volatile uint8 *)(CYDEV_ECC_BASE + idx));
  }

  calcedChecksum = ( uint8 )1u + ( uint8 )(~calcedChecksum);
  CyFlushCache();
 
}

/*******************************************************************************
 * * Function Name: Bootloader_GetMetadata
 * ********************************************************************************
 * *
 * * Summary:
 * *  Returns the value of the specified field of the metadata section.
 * *
 * * Parameters:
 * *  field:
 * *   The field to get data from:
 * *   Bootloader_GET_BTLDB_CHECKSUM    - Bootloadable Application Checksum
 * *   Bootloader_GET_BTLDB_ADDR        - Bootloadable Application Start
 * *                                            Routine Address
 * *   Bootloader_GET_BTLDR_LAST_ROW    - Bootloader Last Flash Row
 * *   Bootloader_GET_BTLDB_LENGTH      - Bootloadable Application Length
 * *   Bootloader_GET_BTLDB_ACTIVE      - Active Bootloadable Application
 * *   Bootloader_GET_BTLDB_STATUS      - Bootloadable Application
 * *                                            Verification Status
 * *   Bootloader_GET_BTLDR_APP_VERSION - Bootloader Application Version
 * *   Bootloader_GET_BTLDB_APP_VERSION - Bootloadable Application Version
 * *   Bootloader_GET_BTLDB_APP_ID      - Bootloadable Application ID
 * *   Bootloader_GET_BTLDB_APP_CUST_ID - Bootloadable Application Custom ID
 * *
 * *  appId:
 * *   Number of the bootlodable application. Should be 0 for the normal
 * *   bootloader and 0 or 1 for the Multi-Application bootloader.
 * *
 * * Return:
 * *  The value of the specified field of the specified application.
 * *
 * *******************************************************************************/
uint32 Bootloader_GetMetadata(uint8 field, uint8 appId)
{
uint32 fieldPtr;
uint8  fieldSize = 2u;
uint32 result = 0u;

switch (field)
{
case Bootloader_GET_BTLDB_CHECKSUM:
fieldPtr  = Bootloader_MD_BTLDB_CHECKSUM_OFFSET(appId);
fieldSize = 1u;
break;

case Bootloader_GET_BTLDB_ADDR:
fieldPtr  = Bootloader_MD_BTLDB_ADDR_OFFSET(appId);
fieldSize = 4u;
break;

case Bootloader_GET_BTLDR_LAST_ROW:
fieldPtr  = Bootloader_MD_BTLDR_LAST_ROW_OFFSET(appId);
break;
case Bootloader_GET_BTLDB_LENGTH:
fieldPtr  = Bootloader_MD_BTLDB_LENGTH_OFFSET(appId);
fieldSize = 4u;
break;

case Bootloader_GET_BTLDB_ACTIVE:
fieldPtr  = Bootloader_MD_BTLDB_ACTIVE_OFFSET(appId);
fieldSize = 1u;
break;
case Bootloader_GET_BTLDB_STATUS:
fieldPtr  = Bootloader_MD_BTLDB_VERIFIED_OFFSET(appId);
fieldSize = 1u;
break;
case Bootloader_GET_BTLDB_APP_VERSION:
fieldPtr  = Bootloader_MD_BTLDB_APP_VERSION_OFFSET(appId);
break;
case Bootloader_GET_BTLDR_APP_VERSION:
fieldPtr  = Bootloader_MD_BTLDR_APP_VERSION_OFFSET(appId);
break;
case Bootloader_GET_BTLDB_APP_ID:
fieldPtr  = Bootloader_MD_BTLDB_APP_ID_OFFSET(appId);
break;

case Bootloader_GET_BTLDB_APP_CUST_ID:
fieldPtr  = Bootloader_MD_BTLDB_APP_CUST_ID_OFFSET(appId);
fieldSize = 4u;
break;

default:
/* Should never be here */
CYASSERT(0u != 0u);
fieldPtr  = 0u;
break;
}


if (1u == fieldSize)
{
result =  (uint32) CY_GET_XTND_REG8((volatile uint8 *)fieldPtr);
}


if (2u == fieldSize)
{
result =  (uint32) CY_GET_XTND_REG8((volatile uint8 *) (fieldPtr     ));
result |= (uint32) CY_GET_XTND_REG8((volatile uint8 *) (fieldPtr + 1u)) <<  8u;
}

if (4u == fieldSize)
{
result =  (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr     ));
result |= (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr + 1u)) <<  8u;
result |= (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr + 2u)) << 16u;
result |= (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr + 3u)) << 24u;
}

return (result);
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

    #if !(CY_PSOC4)
        uint8 arrayId = ( uint8 )(flsAddr / CYDEV_FLS_SECTOR_SIZE);
    #endif  /* !(CY_PSOC4) */

    #if (CY_PSOC4)
        uint16 rowNum = ( uint16 )(flsAddr / CYDEV_FLS_ROW_SIZE);
    #else
        uint16 rowNum = ( uint16 )((flsAddr % CYDEV_FLS_SECTOR_SIZE) / CYDEV_FLS_ROW_SIZE);
    #endif  /* (CY_PSOC4) */

    uint32 baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
    uint16 idx;

    for(idx = 0u; idx < CYDEV_FLS_ROW_SIZE; idx++)
    {
        rowData[idx] = Bootloader_GET_CODE_BYTE(baseAddr + idx);
    }

    rowData[address % CYDEV_FLS_ROW_SIZE] = runType;

    #if(CY_PSOC4)
        (void) CySysFlashWriteRow((uint32) rowNum, rowData);
    #else
        (void) CyWriteRowData(arrayId, rowNum, rowData);
    #endif  /* (CY_PSOC4) */

    #if(CY_PSOC5)
        /***************************************************************************
        * When writing Flash, data in the instruction cache can become stale.
        * Therefore, the cache data does not correlate to the data just written to
        * Flash. A call to CyFlushCache() is required to invalidate the data in the
        * cache and force fresh information to be loaded from Flash.
        ***************************************************************************/
        CyFlushCache();
    #endif /* (CY_PSOC5) */
}

