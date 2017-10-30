/*******************************************************************************
* File Name: .h
* Version 2.50
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_SPIM_0_H)
#define CY_SPIM_PVT_SPIM_0_H

#include "SPIM_0.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 SPIM_0_swStatusTx;
extern volatile uint8 SPIM_0_swStatusRx;

#if(SPIM_0_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 SPIM_0_txBuffer[SPIM_0_TX_BUFFER_SIZE];
    extern volatile uint8 SPIM_0_txBufferRead;
    extern volatile uint8 SPIM_0_txBufferWrite;
    extern volatile uint8 SPIM_0_txBufferFull;
#endif /* (SPIM_0_TX_SOFTWARE_BUF_ENABLED) */

#if(SPIM_0_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 SPIM_0_rxBuffer[SPIM_0_RX_BUFFER_SIZE];
    extern volatile uint8 SPIM_0_rxBufferRead;
    extern volatile uint8 SPIM_0_rxBufferWrite;
    extern volatile uint8 SPIM_0_rxBufferFull;
#endif /* (SPIM_0_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_SPIM_0_H */


/* [] END OF FILE */
