/*******************************************************************************
* File Name: Bootloader.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
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
static uint16   Bootloader_CalcPacketChecksum(const uint8 buffer[], uint16 size) CYSMALL;
static void     Bootloader_HostLink(uint8 timeOut);
static void     Bootloader_LaunchApplication(void) CYSMALL;
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
static uint16 Bootloader_CalcPacketChecksum(const uint8 buffer[], uint16 size)
{
  uint16 CYDATA sum = 0u;
  while (size > 0u)
  {
    sum += buffer[size - 1u];
    size--;
  }
  return(( uint16 )1u + ( uint16 )(~sum));
}


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
uint8 Bootloader_Calc8BitSum(uint32 baseAddr, uint32 start, uint32 size)
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
void Bootloader_Start(void) CYSMALL
{
#if(0u != Bootloader_BOOTLOADER_APP_VALIDATION)
  uint8 CYDATA calcedChecksum;
#endif    /* (0u != Bootloader_BOOTLOADER_APP_VALIDATION) */

#if(0u != Bootloader_FAST_APP_VALIDATION)
#if !defined(CY_BOOT_VERSION)
  /* Not required starting from cy_boot 4.20 */
  uint8 CYXDATA Bootloader_flashBuffer[Bootloader_FROW_SIZE];
#endif /* !defined(CY_BOOT_VERSION) */
#endif  /* (0u != Bootloader_FAST_APP_VALIDATION) */

  cystatus validApp  = CYRET_BAD_DATA;

  /* Validate current resident application */
  if (CYRET_SUCCESS == Bootloader_ValidateBootloadable(Bootloader_MD_BTLDB_ACTIVE_0))
  {
    validApp = CYRET_SUCCESS;
  }
  
/* Initialize Flash subsystem for non-PSoC 4 devices */
#if(0u != Bootloader_FAST_APP_VALIDATION)
  if (CYRET_SUCCESS != CySetTemp())
  {
    CyHalt(0x00u);
  }
#if !defined(CY_BOOT_VERSION)
/* Not required with cy_boot 4.20 */
  if (CYRET_SUCCESS != CySetFlashEEBuffer(Bootloader_flashBuffer))
  {
    CyHalt(0x00u);
  }
#endif /* !defined(CY_BOOT_VERSION) */
#endif  /* (0u != Bootloader_FAST_APP_VALIDATION) */

/***********************************************************************
* Bootloader Application Validation
*
* Halt the device if:
*  - A calculated checksum does not match the one stored in the metadata
*     section.
*  - There is an invalid pointer to the place where the bootloader
*    application ends.
*  - Flash subsystem was not initialized correctly
***********************************************************************/
#if(0u != Bootloader_BOOTLOADER_APP_VALIDATION)
  /* Calculate Bootloader application checksum */
  calcedChecksum = Bootloader_Calc8BitSum(CY_FLASH_BASE,
  Bootloader_MD_BTLDR_ADDR_PTR,
  *Bootloader_SizeBytesAccess - Bootloader_MD_BTLDR_ADDR_PTR);

  /* we included checksum, so remove it */
  calcedChecksum -= *Bootloader_ChecksumAccess;
  calcedChecksum = ( uint8 )1u + ( uint8 )(~calcedChecksum);

  /* Checksum and pointer to bootloader verification */
  if((calcedChecksum != *Bootloader_ChecksumAccess) ||
     (0u == *Bootloader_SizeBytesAccess))
  {
    CyHalt(0x00u);
  }
#endif  /* (0u != Bootloader_BOOTLOADER_APP_VALIDATION) */

    /***********************************************************************
    * If the active bootloadable application is invalid or a bootloader
    * application is scheduled - do the following:
    *  - schedule the bootloader application to be run after software reset
    *  - Go to the communication subroutine. The HostLink() will wait for
    *    the commands forever.
    ***********************************************************************/
    if ((Bootloader_GET_RUN_TYPE == Bootloader_START_BTLDR) ||
        (CYRET_SUCCESS != validApp))
    {
        Bootloader_SET_RUN_TYPE(0u);

        Bootloader_HostLink(Bootloader_WAIT_FOR_COMMAND_FOREVER);
    }


    /* Go to communication subroutine. Will wait for commands for specifed time */
    #if(0u != Bootloader_WAIT_FOR_COMMAND)

        /* Timeout is in 100s of milliseconds */
        Bootloader_HostLink(Bootloader_WAIT_FOR_COMMAND_TIME);

    #endif  /* (0u != Bootloader_WAIT_FOR_COMMAND) */


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
static void Bootloader_LaunchApplication(void) CYSMALL `=ReentrantKeil("Bootloader_LaunchApplication")`
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
                Bootloader_SetFlashByte((uint32) Bootloader_MD_BTLDB_ACTIVE_OFFSET(idx),
                                              (uint8 )(idx == appId));
            }

        #if(0u != Bootloader_DUAL_APP_BOOTLOADER)
            Bootloader_activeApp = appId;
        #endif /* (0u != Bootloader_DUAL_APP_BOOTLOADER) */

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
        Bootloader_LaunchBootloadable(0x00003811);
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
cystatus Bootloader_ValidateBootloadable(uint8 appId) CYSMALL
    {
        uint32 CYDATA idx;

        uint32 CYDATA end   = Bootloader_FIRST_APP_BYTE(appId) +
                                Bootloader_GetMetadata(Bootloader_GET_BTLDB_LENGTH,
                                                       appId);

        CYBIT         valid = 0u; /* Assume bad flash image */
        uint8  CYDATA calcedChecksum = 0u;


        #if(0u != Bootloader_DUAL_APP_BOOTLOADER)

            if(appId > 1u)
            {
                return(CYRET_BAD_DATA);
            }

        #endif  /* (0u != Bootloader_DUAL_APP_BOOTLOADER) */


        #if(0u != Bootloader_FAST_APP_VALIDATION)


            if(Bootloader_GetMetadata(Bootloader_GET_BTLDB_STATUS, appId) ==
               Bootloader_MD_BTLDB_IS_VERIFIED)
            {
                return(CYRET_SUCCESS);
            }

        #endif  /* (0u != Bootloader_FAST_APP_VALIDATION) */


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


        #if((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0u))

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

        #endif  /* ((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0u)) */


        calcedChecksum = ( uint8 )1u + ( uint8 )(~calcedChecksum);


        if((calcedChecksum != Bootloader_GetMetadata(Bootloader_GET_BTLDB_CHECKSUM, appId)) ||
           (0u == valid))
        {
            return(CYRET_BAD_DATA);
        }


        #if(0u != Bootloader_FAST_APP_VALIDATION)
            Bootloader_SetFlashByte((uint32) Bootloader_MD_BTLDB_VERIFIED_OFFSET(appId),
                                          Bootloader_MD_BTLDB_IS_VERIFIED);
        #endif  /* (0u != Bootloader_FAST_APP_VALIDATION) */


        return(CYRET_SUCCESS);
}


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
static void Bootloader_HostLink(uint8 timeOut)
{
#if(0u == Bootloader_FAST_APP_VALIDATION)
#if !defined(CY_BOOT_VERSION)
  /* Not required with cy_boot 4.20 */
  uint8 CYXDATA Bootloader_flashBuffer[Bootloader_FROW_SIZE];
#endif /* !defined(CY_BOOT_VERSION) */
#endif  /* (0u == Bootloader_FAST_APP_VALIDATION) */



#if(0u == Bootloader_FAST_APP_VALIDATION)
  /* Initialize Flash subsystem for non-PSoC 4 devices */
  if (CYRET_SUCCESS != CySetTemp())
  {
      CyHalt(0x00u);
  }
#endif  /* (0u == Bootloader_FAST_APP_VALIDATION) */

  /* Initialize communications channel. */
  CyBtldrCommStart();

  static const uint8 deviceID[7] = {'A','V','R','B', 'O', 'O', 'T'};
  static const uint8 swID[2] = {'1', '0'};
  static const uint8 hwID[2] = {'1', '0'};
  uint16 bytesReceived = 0;
  uint8 inputBuffer[270];
  uint8 outputBuffer[256];
  uint32 currentAddress = 0x00000000;
  uint16 bytesToRead = 0;
  uint16 bytesExpected = 0;
  uint16 temp = 0;
  uint8 bootloadingStarted = 1;
  
  for(;;)
  {
    if (USBUART_DataIsReady())
    {
      bytesReceived = USBUART_GetAll(inputBuffer);
      if (bytesReceived > 0)
      {
        switch(inputBuffer[0])
        {
          case 'A':
            while (bytesReceived < 3)
            {
              if ( USBUART_DataIsReady() )
              {
                bytesReceived += USBUART_GetAll(inputBuffer + bytesReceived);
              }
            }
            currentAddress = inputBuffer[1] << 9;
            currentAddress |= inputBuffer[2] << 1;
            outputBuffer[0] = '\r';
            UART_PutArray(inputBuffer+1, 2);
            sendResponse(outputBuffer, 1);
            
            if (bootloadingStarted != 0)
            {
              writeToFlash(&currentAddress, CY_FLASH_SIZE - 63, 4);
              bootloadingStarted = 0;
            }
            break;
          case 'g':
            while (bytesReceived < 4)
            {
              if ( USBUART_DataIsReady() )
              {
                bytesReceived += USBUART_GetAll(inputBuffer + bytesReceived);
              }
            }
            bytesToRead = inputBuffer[1] << 8;
            bytesToRead |= inputBuffer[2];
            uint16 i = 0;
            for (i = 0; i<bytesToRead; ++i)
            {
              sendResponse((uint8*)(currentAddress + i), 1);
            }
            break;
          case 'B':
            /* When we enter here, we've received some number of bytes, up
             * to 64. If we've received *at least* 4, we have all the info
             * we need to proceed. */
            temp = bytesReceived;
            while (bytesReceived < 5)
            {
              /* If we've received less than 4, we need to collect data until
               * we have at least 4, so we have all we need to figure out how
               * many we're ultimately looking for. */
              if ( USBUART_DataIsReady() )
              {
                bytesReceived += USBUART_GetAll(inputBuffer + bytesReceived);
              }
            }
            bytesExpected = inputBuffer[1]<<8 | inputBuffer[2];
            while (bytesReceived < bytesExpected + 4)
            {
              if ( USBUART_DataIsReady() )
              {
                bytesReceived += USBUART_GetAll(inputBuffer + bytesReceived);
              }
            }
            UART_PutArray(inputBuffer, (uint8)(bytesReceived-temp));
            writeToFlash(inputBuffer+4, currentAddress, bytesExpected);
            currentAddress += 256;
            outputBuffer[0] = '\r';
            sendResponse(outputBuffer, 1);
            break;
          case 'S':
            sendResponse(deviceID, 8);
            break;
          case 'V':
            sendResponse(swID, 2);
            break;
          case 'p':
            outputBuffer[0] = 'S';
            sendResponse(outputBuffer, 1);
            break;
          case 'a':
            outputBuffer[0] = 'Y';
            sendResponse(outputBuffer, 1);
            break;
          case 'b':
            outputBuffer[0] = 'Y';
            outputBuffer[1] = 0x01;
            outputBuffer[2] = 0x00;
            sendResponse(outputBuffer, 3);
            break;
          case 'v':
            sendResponse(hwID, 2);
            break;
          case 't':
            outputBuffer[0] = 0x44;
            outputBuffer[1] = 0;
            sendResponse(outputBuffer, 2);
            break;
          case 's':
            outputBuffer[0] = 0x87;
            outputBuffer[1] = 0x95;
            outputBuffer[2] = 0x1e;
            sendResponse(outputBuffer, 3);
            break;
          case 'F':
          case 'N':
          case 'Q':
          case 'r':
            outputBuffer[0] = 0x00;
            sendResponse(outputBuffer, 1);
            break;
          case 'T':
          case 'x':
          case 'y':
          case 'P':
            outputBuffer[0] = '\r';
            sendResponse(outputBuffer, 1);
            break;
          case 'L':
          case 'E':
            outputBuffer[0] = '\r';
            sendResponse(outputBuffer, 1);   
            LED_PWM_Stop();             
            //if(CYRET_SUCCESS == Bootloader_ValidateBootloadable(Bootloader_activeApp))
              {
                //Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
              }
              CyDelay(1000);
              Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
              Bootloader_LaunchBootloadable(0x00003800);
              break; // should never see this...
          default:
            UART_PutChar('?');
            while (USBUART_CDCIsReady() == 0)
            { /* wait for the CDC to be ready to ship out data */ }
            USBUART_PutChar('?');
        }
        bytesReceived = 0;
      }
    }
  }
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
void Bootloader_SetFlashByte(uint32 address, uint8 runType) `=ReentrantKeil("Bootloader_SetFlashByte")`
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


/*******************************************************************************
* Function Name: Bootloader_GetMetadata
********************************************************************************
*
* Summary:
*  Returns the value of the specified field of the metadata section.
*
* Parameters:
*  field:
*   The field to get data from:
*   Bootloader_GET_BTLDB_CHECKSUM    - Bootloadable Application Checksum
*   Bootloader_GET_BTLDB_ADDR        - Bootloadable Application Start
*                                            Routine Address
*   Bootloader_GET_BTLDR_LAST_ROW    - Bootloader Last Flash Row
*   Bootloader_GET_BTLDB_LENGTH      - Bootloadable Application Length
*   Bootloader_GET_BTLDB_ACTIVE      - Active Bootloadable Application
*   Bootloader_GET_BTLDB_STATUS      - Bootloadable Application
*                                            Verification Status
*   Bootloader_GET_BTLDR_APP_VERSION - Bootloader Application Version
*   Bootloader_GET_BTLDB_APP_VERSION - Bootloadable Application Version
*   Bootloader_GET_BTLDB_APP_ID      - Bootloadable Application ID
*   Bootloader_GET_BTLDB_APP_CUST_ID - Bootloadable Application Custom ID
*
*  appId:
*   Number of the bootlodable application. Should be 0 for the normal
*   bootloader and 0 or 1 for the Multi-Application bootloader.
*
* Return:
*  The value of the specified field of the specified application.
*
*******************************************************************************/
uint32 Bootloader_GetMetadata(uint8 field, uint8 appId)`=ReentrantKeil("Bootloader_GetMetadata")`
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
    #if(!CY_PSOC3)
        fieldSize = 4u;
    #endif  /* (!CY_PSOC3) */
        break;

    case Bootloader_GET_BTLDR_LAST_ROW:
        fieldPtr  = Bootloader_MD_BTLDR_LAST_ROW_OFFSET(appId);
        break;

    case Bootloader_GET_BTLDB_LENGTH:
        fieldPtr  = Bootloader_MD_BTLDB_LENGTH_OFFSET(appId);
    #if(!CY_PSOC3)
        fieldSize = 4u;
    #endif  /* (!CY_PSOC3) */
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

    #if(CY_PSOC3)   /* Big-endian */

        if (2u == fieldSize)
        {
            result =  (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr + 1u));
            result |= (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr     )) <<  8u;
        }

        if (4u == fieldSize)
        {
            result =  (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr + 3u));
            result |= (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr + 2u)) <<  8u;
            result |= (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr + 1u)) << 16u;
            result |= (uint32) CY_GET_XTND_REG8((volatile uint8 *)(fieldPtr     )) << 24u;
        }

    #else   /* PSoC 4 and PSoC 5: Little-endian */

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

    #endif /* (CY_PSOC3) */

    return (result);
}

void sendResponse(const uint8* buffer, uint16 len)
{
  while (USBUART_CDCIsReady() == 0)
  { /* wait for the CDC to be ready to ship out data */ }
  USBUART_PutData(buffer, len);
  //UART_PutArray(buffer, len);
}

void writeToFlash(uint8* data, uint32 address, uint16 dataLen) 
{
  const uint32 flsRowSize = 0x000000100;
  const uint32 flsSectorSize = 0x00010000;

  uint8 arrayId = ( uint8 )(address / flsSectorSize);

  uint16 rowNum = ( uint16 )((address % flsSectorSize) / flsRowSize);

  if (dataLen < flsRowSize)
  {
    uint8 memBuffer[256];
    memcpy(memBuffer, (const void*) address, 256);
    memcpy(memBuffer, data, dataLen);
  }

  CyWriteRowFull(arrayId, rowNum, data, flsRowSize); 

  /***************************************************************************
  * * When writing Flash, data in the instruction cache can become stale.
  * * Therefore, the cache data does not correlate to the data just written to
  * * Flash. A call to CyFlushCache() is required to invalidate the data in the
  * * cache and force fresh information to be loaded from Flash.
  * **************************************************************************/

  CyFlushCache();

}

/* [] END OF FILE */
