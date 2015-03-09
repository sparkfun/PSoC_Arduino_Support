/*******************************************************************************
* File Name: Bootloader.h
* Version 1.30
*
*  Description:
*   Provides an API for the Bootloader. The API includes functions for starting
*   boot loading operations, validating the application and jumping to the
*   application.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_BOOTLOADER_Bootloader_H)
#define CY_BOOTLOADER_Bootloader_H

#include "cytypes.h"
#include "CyFlash.h"

/*******************************************************************************
* Bootloadable applications identification
*******************************************************************************/
#define Bootloader_MD_BTLDB_ACTIVE_0          (0x00u)

/* Mask used to indicate starting application */
#define Bootloader_SCHEDULE_BTLDB             (0x80u)
#define Bootloader_SCHEDULE_BTLDR             (0x40u)
#define Bootloader_SCHEDULE_MASK              (0xC0u)

__attribute__((section (".bootloader")))

extern const uint8  CYCODE Bootloader_Checksum;
extern const uint8  CYCODE  *Bootloader_ChecksumAccess;

__attribute__((section (".bootloader")))

extern const uint32 CYCODE Bootloader_SizeBytes;
extern const uint32 CYCODE *Bootloader_SizeBytesAccess;

#define Bootloader_RESET_SR0_REG     (* (reg8*) CYREG_RESET_SR0 )
#define Bootloader_RESET_SR0_PTR      (reg8*) CYREG_RESET_SR0 

/*******************************************************************************
* Get the reason of the device reset
*  Return cyBtldrRunType in the case if software reset was the reset reason and
*  set cyBtldrRunType to zero (bootloader application is scheduled - that is
*  the initial clean state) and return zero.
*******************************************************************************/

#define Bootloader_GET_RUN_TYPE  (Bootloader_RESET_SR0_REG & Bootloader_SCHEDULE_MASK)

/*******************************************************************************
* Schedule Bootloader/Bootloadable to be run after software reset
*******************************************************************************/

#define Bootloader_SET_RUN_TYPE(x)  (Bootloader_RESET_SR0_REG = (x))

/* Returns the number of Flash arrays available in the device */
#ifndef CY_FLASH_NUMBER_ARRAYS
    #define CY_FLASH_NUMBER_ARRAYS  (CYDEV_FLASH_SIZE / CYDEV_FLS_SECTOR_SIZE)
#endif /* CY_FLASH_NUMBER_ARRAYS */


/*******************************************************************************
* External References
*******************************************************************************/
void Bootloader_SetFlashByte(uint32 address, uint8 runType);
void CyBtldr_CheckLaunch(void)  CYSMALL ;
void Bootloader_LaunchApplication(void) CYSMALL;
void Bootloader_Start(cystatus *validApp) CYSMALL ;
cystatus Bootloader_ValidateBootloadable(uint8 appId) CYSMALL ;
uint8 Bootloader_Calc8BitSum(uint32 baseAddr, \
                             uint32 start, \
                             uint32 size) CYSMALL;
uint32   Bootloader_GetMetadata(uint8 field, uint8 appId);
void Bootloader_Exit(uint8 appId) CYSMALL ;

/*******************************************************************************
* Bootloader_GetMetadata()
*******************************************************************************/
#define Bootloader_GET_BTLDB_CHECKSUM         (1u)
#define Bootloader_GET_BTLDB_ADDR             (2u)
#define Bootloader_GET_BTLDR_LAST_ROW         (3u)
#define Bootloader_GET_BTLDB_LENGTH           (4u)
#define Bootloader_GET_BTLDB_ACTIVE           (5u)
#define Bootloader_GET_BTLDB_STATUS           (6u)
#define Bootloader_GET_BTLDR_APP_VERSION      (7u)
#define Bootloader_GET_BTLDB_APP_VERSION      (8u)
#define Bootloader_GET_BTLDB_APP_ID           (9u)
#define Bootloader_GET_BTLDB_APP_CUST_ID      (10u)

#define Bootloader_GET_METADATA_RESPONSE_SIZE (56u)

/*******************************************************************************
* Bootloader_Exit()
*******************************************************************************/
#define Bootloader_EXIT_TO_BTLDR              (2u)
#define Bootloader_EXIT_TO_BTLDB              (0u)
#define Bootloader_START_APP                  (0u)

#endif /* CY_BOOTLOADER_Bootloader_H */

/* [] END OF FILE */

