/*******************************************************************************
* File Name: `$INSTANCE_NAME`_PVT.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
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

#if !defined(CY_BOOTLOADER_`$INSTANCE_NAME`_PVT_H)
#define CY_BOOTLOADER_`$INSTANCE_NAME`_PVT_H

#include "`$INSTANCE_NAME`.h"


typedef struct
{
    uint32 SiliconId;
    uint8  Revision;
    uint8  BootLoaderVersion[3u];

} `$INSTANCE_NAME`_ENTER;


#define `$INSTANCE_NAME`_VERSION        {\
                                            (uint8)`$CY_MINOR_VERSION`, \
                                            (uint8)`$CY_MAJOR_VERSION`, \
                                            (uint8)0x01u \
                                        }

/* Packet framing constants. */
#define `$INSTANCE_NAME`_SOP            (0x01u)    /* Start of Packet */
#define `$INSTANCE_NAME`_EOP            (0x17u)    /* End of Packet */


/* Bootloader command responses */
#define `$INSTANCE_NAME`_ERR_KEY       (0x01u)  /* The provided key does not match the expected value          */
#define `$INSTANCE_NAME`_ERR_VERIFY    (0x02u)  /* The verification of flash failed                            */
#define `$INSTANCE_NAME`_ERR_LENGTH    (0x03u)  /* The amount of data available is outside the expected range  */
#define `$INSTANCE_NAME`_ERR_DATA      (0x04u)  /* The data is not of the proper form                          */
#define `$INSTANCE_NAME`_ERR_CMD       (0x05u)  /* The command is not recognized                               */
#define `$INSTANCE_NAME`_ERR_DEVICE    (0x06u)  /* The expected device does not match the detected device      */
#define `$INSTANCE_NAME`_ERR_VERSION   (0x07u)  /* The bootloader version detected is not supported            */
#define `$INSTANCE_NAME`_ERR_CHECKSUM  (0x08u)  /* The checksum does not match the expected value              */
#define `$INSTANCE_NAME`_ERR_ARRAY     (0x09u)  /* The flash array is not valid                                */
#define `$INSTANCE_NAME`_ERR_ROW       (0x0Au)  /* The flash row is not valid                                  */
#define `$INSTANCE_NAME`_ERR_PROTECT   (0x0Bu)  /* The flash row is protected and can not be programmed        */
#define `$INSTANCE_NAME`_ERR_APP       (0x0Cu)  /* The application is not valid and cannot be set as active    */
#define `$INSTANCE_NAME`_ERR_ACTIVE    (0x0Du)  /* The application is currently marked as active               */
#define `$INSTANCE_NAME`_ERR_UNK       (0x0Fu)  /* An unknown error occurred                                   */


/* Bootloader command definitions. */
#define `$INSTANCE_NAME`_COMMAND_CHECKSUM     (0x31u)    /* Verify the checksum for the bootloadable project   */
#define `$INSTANCE_NAME`_COMMAND_REPORT_SIZE  (0x32u)    /* Report the programmable portions of flash          */
#define `$INSTANCE_NAME`_COMMAND_APP_STATUS   (0x33u)    /* Gets status info about the provided app status     */
#define `$INSTANCE_NAME`_COMMAND_ERASE        (0x34u)    /* Erase the specified flash row                      */
#define `$INSTANCE_NAME`_COMMAND_SYNC         (0x35u)    /* Sync the bootloader and host application           */
#define `$INSTANCE_NAME`_COMMAND_APP_ACTIVE   (0x36u)    /* Sets the active application                        */
#define `$INSTANCE_NAME`_COMMAND_DATA         (0x37u)    /* Queue up a block of data for programming           */
#define `$INSTANCE_NAME`_COMMAND_ENTER        (0x38u)    /* Enter the bootloader                               */
#define `$INSTANCE_NAME`_COMMAND_PROGRAM      (0x39u)    /* Program the specified row                          */
#define `$INSTANCE_NAME`_COMMAND_VERIFY       (0x3Au)    /* Compute flash row checksum for verification        */
#define `$INSTANCE_NAME`_COMMAND_EXIT         (0x3Bu)    /* Exits the bootloader & resets the chip             */
#define `$INSTANCE_NAME`_COMMAND_GET_METADATA (0x3Cu)    /* Reports the metadata for a selected application    */


/*******************************************************************************
* Bootloader packet byte addresses:
* [1-byte] [1-byte ] [2-byte] [n-byte] [ 2-byte ] [1-byte]
* [ SOP  ] [Command] [ Size ] [ Data ] [Checksum] [ EOP  ]
*******************************************************************************/
#define `$INSTANCE_NAME`_SOP_ADDR             (0x00u)         /* Start of packet offset from beginning     */
#define `$INSTANCE_NAME`_CMD_ADDR             (0x01u)         /* Command offset from beginning             */
#define `$INSTANCE_NAME`_SIZE_ADDR            (0x02u)         /* Packet size offset from beginning         */
#define `$INSTANCE_NAME`_DATA_ADDR            (0x04u)         /* Packet data offset from beginning         */
#define `$INSTANCE_NAME`_CHK_ADDR(x)          (0x04u + (x))   /* Packet checksum offset from end           */
#define `$INSTANCE_NAME`_EOP_ADDR(x)          (0x06u + (x))   /* End of packet offset from end             */
#define `$INSTANCE_NAME`_MIN_PKT_SIZE         (7u)            /* The minimum number of bytes in a packet   */


/*******************************************************************************
`$INSTANCE_NAME`_ValidateBootloadable()
*******************************************************************************/
#define `$INSTANCE_NAME`_FIRST_APP_BYTE(appId)      ((uint32)CYDEV_FLS_ROW_SIZE * \
        ((uint32) `$INSTANCE_NAME`_GetMetadata(`$INSTANCE_NAME`_GET_BTLDR_LAST_ROW, appId) + \
         (uint32) 1u))

#define `$INSTANCE_NAME`_MD_BTLDB_IS_VERIFIED       (0x01u)


/*******************************************************************************
* `$INSTANCE_NAME`_Start()
*******************************************************************************/
#define `$INSTANCE_NAME`_MD_BTLDB_IS_ACTIVE         (0x01u)
#define `$INSTANCE_NAME`_WAIT_FOR_COMMAND_FOREVER   (0x00u)


 /* The maximum number of bytes accepted in a packet plus some */
#define `$INSTANCE_NAME`_SIZEOF_COMMAND_BUFFER      (300u)


/*******************************************************************************
* `$INSTANCE_NAME`_HostLink()
*******************************************************************************/
#define `$INSTANCE_NAME`_COMMUNICATION_STATE_IDLE   (0u)
#define `$INSTANCE_NAME`_COMMUNICATION_STATE_ACTIVE (1u)

#if(!CY_PSOC4)

    /*******************************************************************************
    * The Array ID indicates the unique ID of the SONOS array being accessed:
    * - 0x00-0x3E : Flash Arrays
    * - 0x3F      : Selects all Flash arrays simultaneously
    * - 0x40-0x7F : Embedded EEPROM Arrays
    *******************************************************************************/
    #define `$INSTANCE_NAME`_FIRST_FLASH_ARRAYID          (0x00u)
    #define `$INSTANCE_NAME`_LAST_FLASH_ARRAYID           (0x3Fu)
    #define `$INSTANCE_NAME`_FIRST_EE_ARRAYID             (0x40u)
    #define `$INSTANCE_NAME`_LAST_EE_ARRAYID              (0x7Fu)

#endif   /* (!CY_PSOC4) */


/*******************************************************************************
* `$INSTANCE_NAME`_CalcPacketChecksum()
*******************************************************************************/
#if(0u != `$INSTANCE_NAME`_PACKET_CHECKSUM_CRC)
    #define `$INSTANCE_NAME`_CRC_CCITT_POLYNOMIAL       (0x8408u)       /* x^16 + x^12 + x^5 + 1 */
    #define `$INSTANCE_NAME`_CRC_CCITT_INITIAL_VALUE    (0xffffu)
#endif /* (0u != `$INSTANCE_NAME`_PACKET_CHECKSUM_CRC) */


/*******************************************************************************
* CyBtldr_CheckLaunch()
*******************************************************************************/
#define `$INSTANCE_NAME`_RES_CAUSE_RESET_SOFT                (0x10u)


/*******************************************************************************
* Metadata addresses and pointer defines
*******************************************************************************/
#define `$INSTANCE_NAME`_MD_SIZEOF                  (64u)


/*******************************************************************************
* The Metadata base address. In the case of the bootloader application, the
* metadata is placed at row N-1; in the case of the multi-application
* bootloader, the bootloadable application number 1 will use row N-1, and
* application number 2 will use row N-2 to store its metadata, where N is the
* total number of the rows for the selected device.
*******************************************************************************/
#define `$INSTANCE_NAME`_MD_BASE_ADDR(appId)        (CYDEV_FLASH_BASE + \
                                                        (CYDEV_FLASH_SIZE - ((uint32)(appId) * CYDEV_FLS_ROW_SIZE) - \
                                                        `$INSTANCE_NAME`_MD_SIZEOF))

#define `$INSTANCE_NAME`_MD_FLASH_ARRAY_NUM         (`$INSTANCE_NAME`_NUM_OF_FLASH_ARRAYS - 1u)

#if(!CY_PSOC4)
    #define `$INSTANCE_NAME`_MD_ROW_NUM(appId)      ((CY_FLASH_NUMBER_ROWS / `$INSTANCE_NAME`_NUM_OF_FLASH_ARRAYS) - \
                                                    1u - (uint32)(appId))
#else
    #define `$INSTANCE_NAME`_MD_ROW_NUM(appId)      (CY_FLASH_NUMBER_ROWS - 1u - (uint32)(appId))
#endif /* (!CY_PSOC4) */


#define     `$INSTANCE_NAME`_MD_BTLDB_CHECKSUM_OFFSET(appId)       (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 0u)
#if(CY_PSOC3)
    #define `$INSTANCE_NAME`_MD_BTLDB_ADDR_OFFSET(appId)           (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 3u)
    #define `$INSTANCE_NAME`_MD_BTLDR_LAST_ROW_OFFSET(appId)       (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 7u)
    #define `$INSTANCE_NAME`_MD_BTLDB_LENGTH_OFFSET(appId)         (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 11u)
#else
    #define `$INSTANCE_NAME`_MD_BTLDB_ADDR_OFFSET(appId)           (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 1u)
    #define `$INSTANCE_NAME`_MD_BTLDR_LAST_ROW_OFFSET(appId)       (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 5u)
    #define `$INSTANCE_NAME`_MD_BTLDB_LENGTH_OFFSET(appId)         (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 9u)
#endif /* (CY_PSOC3) */
#define     `$INSTANCE_NAME`_MD_BTLDB_ACTIVE_OFFSET(appId)         (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 16u)
#define     `$INSTANCE_NAME`_MD_BTLDB_VERIFIED_OFFSET(appId)       (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 17u)
#define     `$INSTANCE_NAME`_MD_BTLDR_APP_VERSION_OFFSET(appId)    (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 18u)
#define     `$INSTANCE_NAME`_MD_BTLDB_APP_ID_OFFSET(appId)         (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 20u)
#define     `$INSTANCE_NAME`_MD_BTLDB_APP_VERSION_OFFSET(appId)    (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 22u)
#define     `$INSTANCE_NAME`_MD_BTLDB_APP_CUST_ID_OFFSET(appId)    (`$INSTANCE_NAME`_MD_BASE_ADDR(appId) + 24u)


/*******************************************************************************
* Get data byte from FLASH
*******************************************************************************/
#if(CY_PSOC3)
    #define `$INSTANCE_NAME`_GET_CODE_BYTE(addr)            (*((uint8  CYCODE *) (addr)))
#else
    #define `$INSTANCE_NAME`_GET_CODE_BYTE(addr)            (*((uint8  *)(CYDEV_FLASH_BASE + (addr))))
#endif /* (CY_PSOC3) */


#if(!CY_PSOC4)
    #define `$INSTANCE_NAME`_GET_EEPROM_BYTE(addr)          (*((uint8  *)(CYDEV_EE_BASE + (addr))))
#endif /* (CY_PSOC3) */


/* Our definition of a row size. */
#if((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0))
    #define `$INSTANCE_NAME`_FROW_SIZE          ((CYDEV_FLS_ROW_SIZE) + (CYDEV_ECC_ROW_SIZE))
#else
    #define `$INSTANCE_NAME`_FROW_SIZE          CYDEV_FLS_ROW_SIZE
#endif  /* ((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0)) */


/*******************************************************************************
* Number of addresses remapped from Flash to RAM, when interrupt vectors are
* configured to be stored in RAM (default setting, configured by cy_boot).
*******************************************************************************/
#if(CY_PSOC4)
    #define `$INSTANCE_NAME`_MD_BTLDR_ADDR_PTR        (0xC0u)     /* Exclude the vector */
#else
    #define `$INSTANCE_NAME`_MD_BTLDR_ADDR_PTR        (0x00u)
#endif  /* (CY_PSOC4) */


/*******************************************************************************
* The maximum number of Bootloadable applications
*******************************************************************************/
#if(1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)
    #define `$INSTANCE_NAME`_MAX_NUM_OF_BTLDB       (0x02u)
#else
    #define `$INSTANCE_NAME`_MAX_NUM_OF_BTLDB       (0x01u)
#endif  /* (1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


/*******************************************************************************
* Returns TRUE if the row specified as a parameter contains a metadata section
*******************************************************************************/
#if(0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)
    #define `$INSTANCE_NAME`_CONTAIN_METADATA(row)  \
                                        ((`$INSTANCE_NAME`_MD_ROW_NUM(`$INSTANCE_NAME`_MD_BTLDB_ACTIVE_0) == (row)) || \
                                         (`$INSTANCE_NAME`_MD_ROW_NUM(`$INSTANCE_NAME`_MD_BTLDB_ACTIVE_1) == (row)))
#else
    #define `$INSTANCE_NAME`_CONTAIN_METADATA(row)  \
                                        (`$INSTANCE_NAME`_MD_ROW_NUM(`$INSTANCE_NAME`_MD_BTLDB_ACTIVE_0) == (row))
#endif  /* (0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


/*******************************************************************************
* The Metadata section is located in the last flash row for the Boootloader, for
* the Multi-Application Bootloader, the metadata section of the Bootloadable
* application # 0 is located in the last flash row, and the metadata section of
* the Bootloadable application # 1 is located in the flash row before last.
*******************************************************************************/
#if(0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)
    #define `$INSTANCE_NAME`_GET_APP_ID(row)     \
                                        ((`$INSTANCE_NAME`_MD_ROW_NUM(`$INSTANCE_NAME`_MD_BTLDB_ACTIVE_0) == (row)) ? \
                                          `$INSTANCE_NAME`_MD_BTLDB_ACTIVE_0 : \
                                          `$INSTANCE_NAME`_MD_BTLDB_ACTIVE_1)
#else
    #define `$INSTANCE_NAME`_GET_APP_ID(row)     (`$INSTANCE_NAME`_MD_BTLDB_ACTIVE_0)
#endif  /* (0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


/*******************************************************************************
* Defines the number of flash rows reserved for the metadata section
*******************************************************************************/
#if(0u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)
    #define `$INSTANCE_NAME`_NUMBER_OF_METADATA_ROWS            (1u)
#else
    #define `$INSTANCE_NAME`_NUMBER_OF_METADATA_ROWS            (2u)
#endif /* (0u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


/*******************************************************************************
* Defines the number of possible bootloadable applications
*******************************************************************************/
#if(0u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)
    #define `$INSTANCE_NAME`_NUMBER_OF_BTLDBLE_APPS            (1u)
#else
    #define `$INSTANCE_NAME`_NUMBER_OF_BTLDBLE_APPS            (2u)
#endif /* (0u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */

#define `$INSTANCE_NAME`_NUMBER_OF_ROWS_IN_ARRAY                ((uint16)(CY_FLASH_SIZEOF_ARRAY/CY_FLASH_SIZEOF_ROW))
#define `$INSTANCE_NAME`_FIRST_ROW_IN_ARRAY                     (0u)

#endif /* CY_BOOTLOADER_`$INSTANCE_NAME`_PVT_H */


/* [] END OF FILE */
