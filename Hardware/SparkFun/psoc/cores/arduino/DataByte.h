/*******************************************************************************
* File Name: DataByte.h
* Version 2.30
*
* Description:
*  This header file contains definitions associated with the Shift Register
*  component.
*
* Note: none
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/


#if !defined(CY_SHIFTREG_DataByte_H)
#define CY_SHIFTREG_DataByte_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component ShiftReg_v2_30 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define DataByte_FIFO_SIZE          (4u)
#define DataByte_USE_INPUT_FIFO     (1u)
#define DataByte_USE_OUTPUT_FIFO    (0u)
#define DataByte_SR_SIZE            (24u)


/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;

    uint32 saveSrA0Reg;
    uint32 saveSrA1Reg;

    #if(CY_UDB_V0)
        uint32 saveSrIntMask;
    #endif /* (CY_UDB_V0) */

} DataByte_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  DataByte_Start(void)                              ;
void  DataByte_Stop(void)                               ;
void  DataByte_Init(void)                               ;
void  DataByte_Enable(void)                             ;
void  DataByte_RestoreConfig(void)                      ;
void  DataByte_SaveConfig(void)                         ;
void  DataByte_Sleep(void)                              ;
void  DataByte_Wakeup(void)                             ;
void  DataByte_EnableInt(void)                          ;
void  DataByte_DisableInt(void)                         ;
void  DataByte_SetIntMode(uint8 interruptSource)        ;
uint8 DataByte_GetIntStatus(void)                       ;
void  DataByte_WriteRegValue(uint32 shiftData) \
                                                                ;
uint32 DataByte_ReadRegValue(void) ;
uint8    DataByte_GetFIFOStatus(uint8 fifoId)           ;

#if(0u != DataByte_USE_INPUT_FIFO)
    cystatus DataByte_WriteData(uint32 shiftData) \
                                                                ;
#endif /* (0u != DataByte_USE_INPUT_FIFO) */

#if(0u != DataByte_USE_OUTPUT_FIFO)
    uint32 DataByte_ReadData(void) ;
#endif /* (0u != DataByte_USE_OUTPUT_FIFO) */


/**********************************
*   Variable with external linkage
**********************************/

extern uint8 DataByte_initVar;


/***************************************
*           API Constants
***************************************/

#define DataByte_LOAD                   (0x01u)
#define DataByte_STORE                  (0x02u)
#define DataByte_RESET                  (0x04u)

#define DataByte_IN_FIFO                (0x01u)
#define DataByte_OUT_FIFO               (0x02u)

#define DataByte_RET_FIFO_FULL          (0x00u)

/* This define is obsolete */
#define DataByte_RET_FIFO_NOT_EMPTY     (0x01u)

#define DataByte_RET_FIFO_PARTIAL       (0x01u)
#define DataByte_RET_FIFO_EMPTY         (0x02u)
#define DataByte_RET_FIFO_NOT_DEFINED   (0xFEu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define DataByte__LEFT 0
#define DataByte__RIGHT 1



/***************************************
*    Initial Parameter Constants
***************************************/

#define DataByte_SR_MASK    (0xFFFFFFu) /* Unsigned is added to parameter */
#define DataByte_INT_SRC    (0u)
#define DataByte_DIRECTION  (0u)


/***************************************
*             Registers
***************************************/

/* Control register */
#define DataByte_SR_CONTROL_REG (* (reg8 *) \
                                           DataByte_bSR_SyncCtl_CtrlReg__CONTROL_REG)
#define DataByte_SR_CONTROL_PTR (  (reg8 *) \
                                           DataByte_bSR_SyncCtl_CtrlReg__CONTROL_REG)

/* Status register */
#define DataByte_SR_STATUS_REG      (* (reg8 *) DataByte_bSR_StsReg__STATUS_REG)
#define DataByte_SR_STATUS_PTR      (  (reg8 *) DataByte_bSR_StsReg__STATUS_REG)

/* Interrupt status register */
#define DataByte_SR_STATUS_MASK_REG (* (reg8 *) DataByte_bSR_StsReg__MASK_REG)
#define DataByte_SR_STATUS_MASK_PTR (  (reg8 *) DataByte_bSR_StsReg__MASK_REG)

/* Aux control register */
#define DataByte_SR_AUX_CONTROL_REG (* (reg8 *) DataByte_bSR_StsReg__STATUS_AUX_CTL_REG)
#define DataByte_SR_AUX_CONTROL_PTR (  (reg8 *) DataByte_bSR_StsReg__STATUS_AUX_CTL_REG)

/* A1 register: only used to implement capture function */
#define DataByte_SHIFT_REG_CAPTURE_PTR    ( (reg8 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__A1_REG )

#if(CY_PSOC3 || CY_PSOC5)
    #define DataByte_IN_FIFO_VAL_LSB_PTR        ( (reg32 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__F0_REG )

    #define DataByte_SHIFT_REG_LSB_PTR          ( (reg32 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__A0_REG )

    #define DataByte_SHIFT_REG_VALUE_LSB_PTR    ( (reg32 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__A1_REG )

    #define DataByte_OUT_FIFO_VAL_LSB_PTR       ( (reg32 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__F1_REG )

#else
    #if(DataByte_SR_SIZE <= 8u) /* 8bit - ShiftReg */
        #define DataByte_IN_FIFO_VAL_LSB_PTR        ( (reg8 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__F0_REG )

        #define DataByte_SHIFT_REG_LSB_PTR          ( (reg8 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__A0_REG )

        #define DataByte_SHIFT_REG_VALUE_LSB_PTR    ( (reg8 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__A1_REG )

        #define DataByte_OUT_FIFO_VAL_LSB_PTR       ( (reg8 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__F1_REG )

    #elif(DataByte_SR_SIZE <= 16u) /* 16bit - ShiftReg */
        #define DataByte_IN_FIFO_VAL_LSB_PTR        ( (reg16 *) \
                                  DataByte_bSR_sC24_BShiftRegDp_u0__16BIT_F0_REG )

        #define DataByte_SHIFT_REG_LSB_PTR          ( (reg16 *) \
                                  DataByte_bSR_sC24_BShiftRegDp_u0__16BIT_A0_REG )

        #define DataByte_SHIFT_REG_VALUE_LSB_PTR    ( (reg16 *) \
                                  DataByte_bSR_sC24_BShiftRegDp_u0__16BIT_A1_REG )

        #define DataByte_OUT_FIFO_VAL_LSB_PTR       ( (reg16 *) \
                                  DataByte_bSR_sC24_BShiftRegDp_u0__16BIT_F1_REG )


    #elif(DataByte_SR_SIZE <= 24u) /* 24bit - ShiftReg */
        #define DataByte_IN_FIFO_VAL_LSB_PTR        ( (reg32 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__F0_REG )

        #define DataByte_SHIFT_REG_LSB_PTR          ( (reg32 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__A0_REG )

        #define DataByte_SHIFT_REG_VALUE_LSB_PTR    ( (reg32 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__A1_REG )

        #define DataByte_OUT_FIFO_VAL_LSB_PTR       ( (reg32 *) \
                                        DataByte_bSR_sC24_BShiftRegDp_u0__F1_REG )

    #else /* 32bit - ShiftReg */
        #define DataByte_IN_FIFO_VAL_LSB_PTR        ( (reg32 *) \
                                  DataByte_bSR_sC24_BShiftRegDp_u0__32BIT_F0_REG )

        #define DataByte_SHIFT_REG_LSB_PTR          ( (reg32 *) \
                                  DataByte_bSR_sC24_BShiftRegDp_u0__32BIT_A0_REG )

        #define DataByte_SHIFT_REG_VALUE_LSB_PTR    ( (reg32 *) \
                                  DataByte_bSR_sC24_BShiftRegDp_u0__32BIT_A1_REG )

        #define DataByte_OUT_FIFO_VAL_LSB_PTR       ( (reg32 *) \
                                  DataByte_bSR_sC24_BShiftRegDp_u0__32BIT_F1_REG )

    #endif  /* (DataByte_SR_SIZE <= 8u) */
#endif      /* (CY_PSOC3 || CY_PSOC5) */


/***************************************
*       Register Constants
***************************************/

#define DataByte_INTERRUPTS_ENABLE      (0x10u)
#define DataByte_LOAD_INT_EN            (0x01u)
#define DataByte_STORE_INT_EN           (0x02u)
#define DataByte_RESET_INT_EN           (0x04u)
#define DataByte_CLK_EN                 (0x01u)

#define DataByte_RESET_INT_EN_MASK      (0xFBu)
#define DataByte_LOAD_INT_EN_MASK       (0xFEu)
#define DataByte_STORE_INT_EN_MASK      (0xFDu)
#define DataByte_INTS_EN_MASK           (0x07u)

#define DataByte_OUT_FIFO_CLR_BIT       (0x02u)

#if(0u != DataByte_USE_INPUT_FIFO)

    #define DataByte_IN_FIFO_MASK       (0x18u)

    #define DataByte_IN_FIFO_FULL       (0x00u)
    #define DataByte_IN_FIFO_EMPTY      (0x01u)
    #define DataByte_IN_FIFO_PARTIAL    (0x02u)
    
    /* This define is obsolete */
    #define DataByte_IN_FIFO_NOT_EMPTY  (0x02u)
    
#endif /* (0u != DataByte_USE_INPUT_FIFO) */

#define DataByte_OUT_FIFO_MASK          (0x60u)

#define DataByte_OUT_FIFO_EMPTY         (0x00u)
#define DataByte_OUT_FIFO_FULL          (0x01u)
#define DataByte_OUT_FIFO_PARTIAL       (0x02u)

/* This define is obsolete */
#define DataByte_OUT_FIFO_NOT_EMPTY     (0x02u)

#define DataByte_IN_FIFO_SHIFT_MASK     (0x03u)
#define DataByte_OUT_FIFO_SHIFT_MASK    (0x05u)

#define DataByte_DISABLED               (0u)
#define DataByte_DEFAULT_A0             (0u)
#define DataByte_DEFAULT_A1             (0u)


/***************************************
*       Macros
***************************************/

#define DataByte_IS_ENABLED         (0u != (DataByte_SR_CONTROL & DataByte_CLK_EN))

#define DataByte_GET_OUT_FIFO_STS   ((DataByte_SR_STATUS & DataByte_OUT_FIFO_MASK) >> \
                                              DataByte_OUT_FIFO_SHIFT_MASK)

#define DataByte_GET_IN_FIFO_STS    ((DataByte_SR_STATUS & DataByte_IN_FIFO_MASK)  >> \
                                              DataByte_IN_FIFO_SHIFT_MASK)


/***************************************
*       Obsolete
***************************************/

/* Following code are OBSOLETE and must not be used 
 * starting from ShiftReg 2.20 
 */

#define DataByte_SR_CONTROL     (DataByte_SR_CONTROL_REG)
#define DataByte_SR_STATUS      (DataByte_SR_STATUS_REG)
#define DataByte_SR_STATUS_MASK (DataByte_SR_STATUS_MASK_REG)
#define DataByte_SR_AUX_CONTROL (DataByte_SR_AUX_CONTROL_REG)

#define DataByte_IN_FIFO_SHFT_MASK  (DataByte_IN_FIFO_SHIFT_MASK)
#define DataByte_OUT_FIFO_SHFT_MASK (DataByte_OUT_FIFO_SHFIT_MASK)

#define DataByte_RET_FIFO_BAD_PARAM (0xFFu)

#endif /* (CY_SHIFTREG_DataByte_H) */


/* [] END OF FILE */
