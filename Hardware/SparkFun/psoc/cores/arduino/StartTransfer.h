/*******************************************************************************
* File Name: StartTransfer.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_StartTransfer_H) /* CY_CONTROL_REG_StartTransfer_H */
#define CY_CONTROL_REG_StartTransfer_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} StartTransfer_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    StartTransfer_Write(uint8 control) ;
uint8   StartTransfer_Read(void) ;

void StartTransfer_SaveConfig(void) ;
void StartTransfer_RestoreConfig(void) ;
void StartTransfer_Sleep(void) ; 
void StartTransfer_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define StartTransfer_Control        (* (reg8 *) StartTransfer_Sync_ctrl_reg__CONTROL_REG )
#define StartTransfer_Control_PTR    (  (reg8 *) StartTransfer_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_StartTransfer_H */


/* [] END OF FILE */
