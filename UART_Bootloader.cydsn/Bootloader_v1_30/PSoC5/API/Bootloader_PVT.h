/*******************************************************************************
* File Name: Bootloader_PVT.h
* Version 1.30
*
*  Description:
*   Provides an API for the Bootloader.
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_BOOTLOADER_Bootloader_PVT_H)
#define CY_BOOTLOADER_Bootloader_PVT_H

#include "Bootloader.h"

typedef struct
{
    uint32 SiliconId;
    uint8  Revision;
    uint8  BootLoaderVersion[3u];

} Bootloader_ENTER;

#define Bootloader_VERSION        {\
                                            (uint8)30, \
                                            (uint8)1, \
                                            (uint8)0x01u \
                                        }

/*******************************************************************************
Bootloader_ValidateBootloadable()
*******************************************************************************/
#define Bootloader_FIRST_APP_BYTE(appId)      ((uint32)CYDEV_FLS_ROW_SIZE * \
        ((uint32) Bootloader_GetMetadata(Bootloader_GET_BTLDR_LAST_ROW, appId) + \
         (uint32) 1u))

#define Bootloader_MD_BTLDB_IS_VERIFIED       (0x01u)


/*******************************************************************************
* Bootloader_Start()
*******************************************************************************/
#define Bootloader_MD_BTLDB_IS_ACTIVE         (0x01u)
#define Bootloader_WAIT_FOR_COMMAND_FOREVER   (0x00u)


 /* The maximum number of bytes accepted in a packet plus some */
#define Bootloader_SIZEOF_COMMAND_BUFFER      (0x20Eu)


/*******************************************************************************
* Bootloader_HostLink()
*******************************************************************************/
#define Bootloader_COMMUNICATION_STATE_IDLE   (0u)
#define Bootloader_COMMUNICATION_STATE_ACTIVE (1u)

/*******************************************************************************
* The Array ID indicates the unique ID of the SONOS array being accessed:
* - 0x00-0x3E : Flash Arrays
* - 0x3F      : Selects all Flash arrays simultaneously
* - 0x40-0x7F : Embedded EEPROM Arrays
*******************************************************************************/
#define Bootloader_FIRST_FLASH_ARRAYID          (0x00u)
#define Bootloader_LAST_FLASH_ARRAYID           (0x3Fu)
#define Bootloader_FIRST_EE_ARRAYID             (0x40u)
#define Bootloader_LAST_EE_ARRAYID              (0x7Fu)

/*******************************************************************************
* CyBtldr_CheckLaunch()
*******************************************************************************/
#define Bootloader_RES_CAUSE_RESET_SOFT                (0x10u)


/*******************************************************************************
* Metadata addresses and pointer defines
*******************************************************************************/
#define Bootloader_MD_SIZEOF                  (64u)


/*******************************************************************************
* The Metadata base address. In the case of the bootloader application, the
* metadata is placed at row N-1; in the case of the multi-application
* bootloader, the bootloadable application number 1 will use row N-1, and
* application number 2 will use row N-2 to store its metadata, where N is the
* total number of the rows for the selected device.
*******************************************************************************/
#define Bootloader_MD_BASE_ADDR(appId)        (CYDEV_FLASH_BASE + \
                                                        (CYDEV_FLASH_SIZE - ((uint32)(appId) * CYDEV_FLS_ROW_SIZE) - \
                                                        Bootloader_MD_SIZEOF))

#define Bootloader_MD_FLASH_ARRAY_NUM         (Bootloader_NUM_OF_FLASH_ARRAYS - 1u)


#define Bootloader_MD_ROW_NUM(appId)      ((CY_FLASH_NUMBER_ROWS / Bootloader_NUM_OF_FLASH_ARRAYS) - \
                                                    1u - (uint32)(appId))



#define     Bootloader_MD_BTLDB_CHECKSUM_OFFSET(appId)       (Bootloader_MD_BASE_ADDR(appId) + 0u)
#define     Bootloader_MD_BTLDB_ADDR_OFFSET(appId)           (Bootloader_MD_BASE_ADDR(appId) + 1u)
#define     Bootloader_MD_BTLDR_LAST_ROW_OFFSET(appId)       (Bootloader_MD_BASE_ADDR(appId) + 5u)
#define     Bootloader_MD_BTLDB_LENGTH_OFFSET(appId)         (Bootloader_MD_BASE_ADDR(appId) + 9u)
#define     Bootloader_MD_BTLDB_ACTIVE_OFFSET(appId)         (Bootloader_MD_BASE_ADDR(appId) + 16u)
#define     Bootloader_MD_BTLDB_VERIFIED_OFFSET(appId)       (Bootloader_MD_BASE_ADDR(appId) + 17u)
#define     Bootloader_MD_BTLDR_APP_VERSION_OFFSET(appId)    (Bootloader_MD_BASE_ADDR(appId) + 18u)
#define     Bootloader_MD_BTLDB_APP_ID_OFFSET(appId)         (Bootloader_MD_BASE_ADDR(appId) + 20u)
#define     Bootloader_MD_BTLDB_APP_VERSION_OFFSET(appId)    (Bootloader_MD_BASE_ADDR(appId) + 22u)
#define     Bootloader_MD_BTLDB_APP_CUST_ID_OFFSET(appId)    (Bootloader_MD_BASE_ADDR(appId) + 24u)


/*******************************************************************************
* Get data byte from FLASH
*******************************************************************************/
#define Bootloader_GET_CODE_BYTE(addr)            (*((uint8  *)(CYDEV_FLASH_BASE + (addr))))
#define Bootloader_GET_EEPROM_BYTE(addr)          (*((uint8  *)(CYDEV_EE_BASE + (addr))))

/* Our definition of a row size. */
#define Bootloader_FROW_SIZE          ((CYDEV_FLS_ROW_SIZE) + (CYDEV_ECC_ROW_SIZE))



/*******************************************************************************
* Number of addresses remapped from Flash to RAM, when interrupt vectors are
* configured to be stored in RAM (default setting, configured by cy_boot).
*******************************************************************************/
#define Bootloader_MD_BTLDR_ADDR_PTR        (0x00u)


/*******************************************************************************
* The maximum number of Bootloadable applications
*******************************************************************************/
#define Bootloader_MAX_NUM_OF_BTLDB       (0x01u)

/*******************************************************************************
* Returns TRUE if the row specified as a parameter contains a metadata section
*******************************************************************************/

#define Bootloader_CONTAIN_METADATA(row)  \
                                        (Bootloader_MD_ROW_NUM(Bootloader_MD_BTLDB_ACTIVE_0) == (row))

/*******************************************************************************
* The Metadata section is located in the last flash row for the Boootloader, for
* the Multi-Application Bootloader, the metadata section of the Bootloadable
* application # 0 is located in the last flash row, and the metadata section of
* the Bootloadable application # 1 is located in the flash row before last.
*******************************************************************************/
#define Bootloader_GET_APP_ID(row)     (Bootloader_MD_BTLDB_ACTIVE_0)

/*******************************************************************************
* Defines the number of flash rows reserved for the metadata section
*******************************************************************************/
#define Bootloader_NUMBER_OF_METADATA_ROWS            (1u)

/*******************************************************************************
* Defines the number of possible bootloadable applications
*******************************************************************************/
#define Bootloader_NUMBER_OF_BTLDBLE_APPS            (1u)

#define Bootloader_NUMBER_OF_ROWS_IN_ARRAY                ((uint16)(CY_FLASH_SIZEOF_ARRAY/CY_FLASH_SIZEOF_ROW))
#define Bootloader_FIRST_ROW_IN_ARRAY                     (0u)

#endif /* CY_BOOTLOADER_Bootloader_PVT_H */

/* [] END OF FILE */
