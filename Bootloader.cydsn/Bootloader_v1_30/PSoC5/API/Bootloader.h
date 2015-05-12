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

    
#define APPLICATION_START_ADDRESS 0x00002811 

/*******************************************************************************
* Bootloadable applications identification
*******************************************************************************/
/* Mask used to indicate starting application */
#define Bootloader_SCHEDULE_BTLDB             (0x80u)
#define Bootloader_NEW_BOOT                   (0x00u)
#define Bootloader_SCHEDULE_BTLDR             (0x40u)
#define Bootloader_START_BTLDB                (0x80u)
#define Bootloader_START_BTLDR                (0x40u)
#define Bootloader_SCHEDULE_MASK              (0xC0u)

__attribute__((section (".bootloader")))
extern const uint8  CYCODE Bootloader_Checksum;
extern const uint8  CYCODE  *Bootloader_ChecksumAccess;

__attribute__((section (".bootloader")))
extern const uint32 CYCODE Bootloader_SizeBytes;
extern const uint32 CYCODE *Bootloader_SizeBytesAccess;

/*******************************************************************************
* This variable is used by Bootloader/Bootloadable components to schedule what
* application will be started after software reset.
*******************************************************************************/
#define Bootloader_RESET_SR0_REG           (* (reg8 *) CYREG_RESET_SR0)
#define Bootloader_RESET_SR0_PTR           (  (reg8 *) CYREG_RESET_SR0)

/*******************************************************************************
* Get the reason of the device reset
*  Return cyBtldrRunType in the case if software reset was the reset reason and
*  set cyBtldrRunType to zero (bootloader application is scheduled - that is
*  the initial clean state) and return zero.
*******************************************************************************/
#define Bootloader_GET_RUN_TYPE  \
                        (Bootloader_RESET_SR0_REG & Bootloader_SCHEDULE_MASK)

/*******************************************************************************
* Schedule Bootloader/Bootloadable to be run after software reset
*******************************************************************************/
#define Bootloader_SET_RUN_TYPE(x)          (Bootloader_RESET_SR0_REG = (x))

/* Returns the number of Flash arrays available in the device */
#ifndef CY_FLASH_NUMBER_ARRAYS
    #define CY_FLASH_NUMBER_ARRAYS    (CYDEV_FLASH_SIZE / CYDEV_FLS_SECTOR_SIZE)
#endif /* CY_FLASH_NUMBER_ARRAYS */

/*******************************************************************************
* External References
*******************************************************************************/
void Bootloader_SetFlashByte(uint32 address, uint8 runType);
void CyBtldr_CheckLaunch(void)  CYSMALL ;
void Bootloader_Start(uint32 timeout) CYSMALL ;
void Bootloader_Exit(uint8 appId) CYSMALL ;

#if(CY_PSOC3)
    /* Implementation for the PSoC 3 resides in a Bootloader_psoc3.a51 file.  */
    void     Bootloader_LaunchBootloadable(uint32 appAddr);
#endif  /* (CY_PSOC3) */
extern void CyBtldrCommStart(void);
extern cystatus CyBtldrCommWrite(uint8* buffer, uint16 size, uint16* count, uint8 timeOut);
extern cystatus CyBtldrCommRead (uint8* buffer, uint16 size, uint16* count, uint8 timeOut);

#endif /* CY_BOOTLOADER_Bootloader_H */


/* [] END OF FILE */
