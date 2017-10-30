/*******************************************************************************
* File Name: SPIM_0.h
* Version 2.50
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_SPIM_0_H)
#define CY_SPIM_SPIM_0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component SPI_Master_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define SPIM_0_INTERNAL_CLOCK             (0u)

#if(0u != SPIM_0_INTERNAL_CLOCK)
    #include "SPIM_0_IntClock.h"
#endif /* (0u != SPIM_0_INTERNAL_CLOCK) */

#define SPIM_0_MODE                       (1u)
#define SPIM_0_DATA_WIDTH                 (8u)
#define SPIM_0_MODE_USE_ZERO              (1u)
#define SPIM_0_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define SPIM_0_TX_BUFFER_SIZE             (4u)
#define SPIM_0_RX_BUFFER_SIZE             (4u)
#define SPIM_0_INTERNAL_TX_INT_ENABLED    (0u)
#define SPIM_0_INTERNAL_RX_INT_ENABLED    (0u)

#define SPIM_0_SINGLE_REG_SIZE            (8u)
#define SPIM_0_USE_SECOND_DATAPATH        (SPIM_0_DATA_WIDTH > SPIM_0_SINGLE_REG_SIZE)

#define SPIM_0_FIFO_SIZE                  (4u)
#define SPIM_0_TX_SOFTWARE_BUF_ENABLED    ((0u != SPIM_0_INTERNAL_TX_INT_ENABLED) && \
                                                     (SPIM_0_TX_BUFFER_SIZE > SPIM_0_FIFO_SIZE))

#define SPIM_0_RX_SOFTWARE_BUF_ENABLED    ((0u != SPIM_0_INTERNAL_RX_INT_ENABLED) && \
                                                     (SPIM_0_RX_BUFFER_SIZE > SPIM_0_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
} SPIM_0_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  SPIM_0_Init(void)                           ;
void  SPIM_0_Enable(void)                         ;
void  SPIM_0_Start(void)                          ;
void  SPIM_0_Stop(void)                           ;

void  SPIM_0_EnableTxInt(void)                    ;
void  SPIM_0_EnableRxInt(void)                    ;
void  SPIM_0_DisableTxInt(void)                   ;
void  SPIM_0_DisableRxInt(void)                   ;

void  SPIM_0_Sleep(void)                          ;
void  SPIM_0_Wakeup(void)                         ;
void  SPIM_0_SaveConfig(void)                     ;
void  SPIM_0_RestoreConfig(void)                  ;

void  SPIM_0_SetTxInterruptMode(uint8 intSrc)     ;
void  SPIM_0_SetRxInterruptMode(uint8 intSrc)     ;
uint8 SPIM_0_ReadTxStatus(void)                   ;
uint8 SPIM_0_ReadRxStatus(void)                   ;
void  SPIM_0_WriteTxData(uint8 txData)  \
                                                            ;
uint8 SPIM_0_ReadRxData(void) \
                                                            ;
uint8 SPIM_0_GetRxBufferSize(void)                ;
uint8 SPIM_0_GetTxBufferSize(void)                ;
void  SPIM_0_ClearRxBuffer(void)                  ;
void  SPIM_0_ClearTxBuffer(void)                  ;
void  SPIM_0_ClearFIFO(void)                              ;
void  SPIM_0_PutArray(const uint8 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != SPIM_0_BIDIRECTIONAL_MODE)
    void  SPIM_0_TxEnable(void)                   ;
    void  SPIM_0_TxDisable(void)                  ;
#endif /* (0u != SPIM_0_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(SPIM_0_TX_ISR);
CY_ISR_PROTO(SPIM_0_RX_ISR);


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 SPIM_0_initVar;


/***************************************
*           API Constants
***************************************/

#define SPIM_0_TX_ISR_NUMBER     ((uint8) (SPIM_0_TxInternalInterrupt__INTC_NUMBER))
#define SPIM_0_RX_ISR_NUMBER     ((uint8) (SPIM_0_RxInternalInterrupt__INTC_NUMBER))

#define SPIM_0_TX_ISR_PRIORITY   ((uint8) (SPIM_0_TxInternalInterrupt__INTC_PRIOR_NUM))
#define SPIM_0_RX_ISR_PRIORITY   ((uint8) (SPIM_0_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define SPIM_0_INT_ON_SPI_DONE    ((uint8) (0u   << SPIM_0_STS_SPI_DONE_SHIFT))
#define SPIM_0_INT_ON_TX_EMPTY    ((uint8) (0u   << SPIM_0_STS_TX_FIFO_EMPTY_SHIFT))
#define SPIM_0_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           SPIM_0_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPIM_0_INT_ON_BYTE_COMP   ((uint8) (0u  << SPIM_0_STS_BYTE_COMPLETE_SHIFT))
#define SPIM_0_INT_ON_SPI_IDLE    ((uint8) (0u   << SPIM_0_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define SPIM_0_INT_ON_TX_NOT_FULL_DEF ((SPIM_0_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (SPIM_0_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define SPIM_0_TX_INIT_INTERRUPTS_MASK    (SPIM_0_INT_ON_SPI_DONE  | \
                                                     SPIM_0_INT_ON_TX_EMPTY  | \
                                                     SPIM_0_INT_ON_TX_NOT_FULL_DEF | \
                                                     SPIM_0_INT_ON_BYTE_COMP | \
                                                     SPIM_0_INT_ON_SPI_IDLE)

#define SPIM_0_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          SPIM_0_STS_RX_FIFO_FULL_SHIFT))
#define SPIM_0_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          SPIM_0_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPIM_0_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          SPIM_0_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define SPIM_0_RX_INIT_INTERRUPTS_MASK    (SPIM_0_INT_ON_RX_FULL      | \
                                                     SPIM_0_INT_ON_RX_NOT_EMPTY | \
                                                     SPIM_0_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define SPIM_0_BITCTR_INIT            (((uint8) (SPIM_0_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/
#if(CY_PSOC3 || CY_PSOC5)
    #define SPIM_0_TXDATA_REG (* (reg8 *) \
                                                SPIM_0_BSPIM_sR8_Dp_u0__F0_REG)
    #define SPIM_0_TXDATA_PTR (  (reg8 *) \
                                                SPIM_0_BSPIM_sR8_Dp_u0__F0_REG)
    #define SPIM_0_RXDATA_REG (* (reg8 *) \
                                                SPIM_0_BSPIM_sR8_Dp_u0__F1_REG)
    #define SPIM_0_RXDATA_PTR (  (reg8 *) \
                                                SPIM_0_BSPIM_sR8_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(SPIM_0_USE_SECOND_DATAPATH)
        #define SPIM_0_TXDATA_REG (* (reg16 *) \
                                          SPIM_0_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define SPIM_0_TXDATA_PTR (  (reg16 *) \
                                          SPIM_0_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define SPIM_0_RXDATA_REG (* (reg16 *) \
                                          SPIM_0_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
        #define SPIM_0_RXDATA_PTR (  (reg16 *) \
                                          SPIM_0_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
    #else
        #define SPIM_0_TXDATA_REG (* (reg8 *) \
                                                SPIM_0_BSPIM_sR8_Dp_u0__F0_REG)
        #define SPIM_0_TXDATA_PTR (  (reg8 *) \
                                                SPIM_0_BSPIM_sR8_Dp_u0__F0_REG)
        #define SPIM_0_RXDATA_REG (* (reg8 *) \
                                                SPIM_0_BSPIM_sR8_Dp_u0__F1_REG)
        #define SPIM_0_RXDATA_PTR (  (reg8 *) \
                                                SPIM_0_BSPIM_sR8_Dp_u0__F1_REG)
    #endif /* (SPIM_0_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define SPIM_0_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        SPIM_0_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define SPIM_0_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        SPIM_0_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)

#if(SPIM_0_USE_SECOND_DATAPATH)
    #define SPIM_0_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        SPIM_0_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define SPIM_0_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        SPIM_0_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* (SPIM_0_USE_SECOND_DATAPATH) */

#define SPIM_0_COUNTER_PERIOD_REG     (* (reg8 *) SPIM_0_BSPIM_BitCounter__PERIOD_REG)
#define SPIM_0_COUNTER_PERIOD_PTR     (  (reg8 *) SPIM_0_BSPIM_BitCounter__PERIOD_REG)
#define SPIM_0_COUNTER_CONTROL_REG    (* (reg8 *) SPIM_0_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define SPIM_0_COUNTER_CONTROL_PTR    (  (reg8 *) SPIM_0_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define SPIM_0_TX_STATUS_REG          (* (reg8 *) SPIM_0_BSPIM_TxStsReg__STATUS_REG)
#define SPIM_0_TX_STATUS_PTR          (  (reg8 *) SPIM_0_BSPIM_TxStsReg__STATUS_REG)
#define SPIM_0_RX_STATUS_REG          (* (reg8 *) SPIM_0_BSPIM_RxStsReg__STATUS_REG)
#define SPIM_0_RX_STATUS_PTR          (  (reg8 *) SPIM_0_BSPIM_RxStsReg__STATUS_REG)

#define SPIM_0_CONTROL_REG            (* (reg8 *) \
                                      SPIM_0_BSPIM_BidirMode_CtrlReg__CONTROL_REG)
#define SPIM_0_CONTROL_PTR            (  (reg8 *) \
                                      SPIM_0_BSPIM_BidirMode_CtrlReg__CONTROL_REG)

#define SPIM_0_TX_STATUS_MASK_REG     (* (reg8 *) SPIM_0_BSPIM_TxStsReg__MASK_REG)
#define SPIM_0_TX_STATUS_MASK_PTR     (  (reg8 *) SPIM_0_BSPIM_TxStsReg__MASK_REG)
#define SPIM_0_RX_STATUS_MASK_REG     (* (reg8 *) SPIM_0_BSPIM_RxStsReg__MASK_REG)
#define SPIM_0_RX_STATUS_MASK_PTR     (  (reg8 *) SPIM_0_BSPIM_RxStsReg__MASK_REG)

#define SPIM_0_TX_STATUS_ACTL_REG     (* (reg8 *) SPIM_0_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPIM_0_TX_STATUS_ACTL_PTR     (  (reg8 *) SPIM_0_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPIM_0_RX_STATUS_ACTL_REG     (* (reg8 *) SPIM_0_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define SPIM_0_RX_STATUS_ACTL_PTR     (  (reg8 *) SPIM_0_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(SPIM_0_USE_SECOND_DATAPATH)
    #define SPIM_0_AUX_CONTROLDP1     (SPIM_0_AUX_CONTROL_DP1_REG)
#endif /* (SPIM_0_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define SPIM_0_STS_SPI_DONE_SHIFT             (0x00u)
#define SPIM_0_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define SPIM_0_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define SPIM_0_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define SPIM_0_STS_SPI_IDLE_SHIFT             (0x04u)
#define SPIM_0_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define SPIM_0_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define SPIM_0_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define SPIM_0_STS_SPI_DONE           ((uint8) (0x01u << SPIM_0_STS_SPI_DONE_SHIFT))
#define SPIM_0_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << SPIM_0_STS_TX_FIFO_EMPTY_SHIFT))
#define SPIM_0_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << SPIM_0_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPIM_0_STS_BYTE_COMPLETE      ((uint8) (0x01u << SPIM_0_STS_BYTE_COMPLETE_SHIFT))
#define SPIM_0_STS_SPI_IDLE           ((uint8) (0x01u << SPIM_0_STS_SPI_IDLE_SHIFT))
#define SPIM_0_STS_RX_FIFO_FULL       ((uint8) (0x01u << SPIM_0_STS_RX_FIFO_FULL_SHIFT))
#define SPIM_0_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << SPIM_0_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPIM_0_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << SPIM_0_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define SPIM_0_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define SPIM_0_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define SPIM_0_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define SPIM_0_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define SPIM_0_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define SPIM_0_FIFO_CLR       (SPIM_0_TX_FIFO_CLR | SPIM_0_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define SPIM_0_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define SPIM_0_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define SPIM_0_AUX_CTRL_FIFO0_CLR         (0x01u)
#define SPIM_0_AUX_CTRL_FIFO1_CLR         (0x02u)
#define SPIM_0_AUX_CTRL_FIFO0_LVL         (0x04u)
#define SPIM_0_AUX_CTRL_FIFO1_LVL         (0x08u)
#define SPIM_0_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define SPIM_0_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define SPIM_0_IS_ENABLED (0u != (SPIM_0_TX_STATUS_ACTL_REG & SPIM_0_INT_ENABLE))

/* Retuns TX status register */
#define SPIM_0_GET_STATUS_TX(swTxSts) ( (uint8)(SPIM_0_TX_STATUS_REG | \
                                                          ((swTxSts) & SPIM_0_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define SPIM_0_GET_STATUS_RX(swRxSts) ( (uint8)(SPIM_0_RX_STATUS_REG | \
                                                          ((swRxSts) & SPIM_0_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define SPIM_0_WriteByte   SPIM_0_WriteTxData
#define SPIM_0_ReadByte    SPIM_0_ReadRxData
void  SPIM_0_SetInterruptMode(uint8 intSrc)       ;
uint8 SPIM_0_ReadStatus(void)                     ;
void  SPIM_0_EnableInt(void)                      ;
void  SPIM_0_DisableInt(void)                     ;

#define SPIM_0_TXDATA                 (SPIM_0_TXDATA_REG)
#define SPIM_0_RXDATA                 (SPIM_0_RXDATA_REG)
#define SPIM_0_AUX_CONTROLDP0         (SPIM_0_AUX_CONTROL_DP0_REG)
#define SPIM_0_TXBUFFERREAD           (SPIM_0_txBufferRead)
#define SPIM_0_TXBUFFERWRITE          (SPIM_0_txBufferWrite)
#define SPIM_0_RXBUFFERREAD           (SPIM_0_rxBufferRead)
#define SPIM_0_RXBUFFERWRITE          (SPIM_0_rxBufferWrite)

#define SPIM_0_COUNTER_PERIOD         (SPIM_0_COUNTER_PERIOD_REG)
#define SPIM_0_COUNTER_CONTROL        (SPIM_0_COUNTER_CONTROL_REG)
#define SPIM_0_STATUS                 (SPIM_0_TX_STATUS_REG)
#define SPIM_0_CONTROL                (SPIM_0_CONTROL_REG)
#define SPIM_0_STATUS_MASK            (SPIM_0_TX_STATUS_MASK_REG)
#define SPIM_0_STATUS_ACTL            (SPIM_0_TX_STATUS_ACTL_REG)

#define SPIM_0_INIT_INTERRUPTS_MASK  (SPIM_0_INT_ON_SPI_DONE     | \
                                                SPIM_0_INT_ON_TX_EMPTY     | \
                                                SPIM_0_INT_ON_TX_NOT_FULL_DEF  | \
                                                SPIM_0_INT_ON_RX_FULL      | \
                                                SPIM_0_INT_ON_RX_NOT_EMPTY | \
                                                SPIM_0_INT_ON_RX_OVER      | \
                                                SPIM_0_INT_ON_BYTE_COMP)
                                                
#define SPIM_0_DataWidth                  (SPIM_0_DATA_WIDTH)
#define SPIM_0_InternalClockUsed          (SPIM_0_INTERNAL_CLOCK)
#define SPIM_0_InternalTxInterruptEnabled (SPIM_0_INTERNAL_TX_INT_ENABLED)
#define SPIM_0_InternalRxInterruptEnabled (SPIM_0_INTERNAL_RX_INT_ENABLED)
#define SPIM_0_ModeUseZero                (SPIM_0_MODE_USE_ZERO)
#define SPIM_0_BidirectionalMode          (SPIM_0_BIDIRECTIONAL_MODE)
#define SPIM_0_Mode                       (SPIM_0_MODE)
#define SPIM_0_DATAWIDHT                  (SPIM_0_DATA_WIDTH)
#define SPIM_0_InternalInterruptEnabled   (0u)

#define SPIM_0_TXBUFFERSIZE   (SPIM_0_TX_BUFFER_SIZE)
#define SPIM_0_RXBUFFERSIZE   (SPIM_0_RX_BUFFER_SIZE)

#define SPIM_0_TXBUFFER       SPIM_0_txBuffer
#define SPIM_0_RXBUFFER       SPIM_0_rxBuffer

#endif /* (CY_SPIM_SPIM_0_H) */


/* [] END OF FILE */
