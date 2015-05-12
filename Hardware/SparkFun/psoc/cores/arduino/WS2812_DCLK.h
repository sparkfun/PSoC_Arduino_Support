/*******************************************************************************
* File Name: WS2812_DCLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_WS2812_DCLK_H)
#define CY_CLOCK_WS2812_DCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void WS2812_DCLK_Start(void) ;
void WS2812_DCLK_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void WS2812_DCLK_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void WS2812_DCLK_StandbyPower(uint8 state) ;
void WS2812_DCLK_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 WS2812_DCLK_GetDividerRegister(void) ;
void WS2812_DCLK_SetModeRegister(uint8 modeBitMask) ;
void WS2812_DCLK_ClearModeRegister(uint8 modeBitMask) ;
uint8 WS2812_DCLK_GetModeRegister(void) ;
void WS2812_DCLK_SetSourceRegister(uint8 clkSource) ;
uint8 WS2812_DCLK_GetSourceRegister(void) ;
#if defined(WS2812_DCLK__CFG3)
void WS2812_DCLK_SetPhaseRegister(uint8 clkPhase) ;
uint8 WS2812_DCLK_GetPhaseRegister(void) ;
#endif /* defined(WS2812_DCLK__CFG3) */

#define WS2812_DCLK_Enable()                       WS2812_DCLK_Start()
#define WS2812_DCLK_Disable()                      WS2812_DCLK_Stop()
#define WS2812_DCLK_SetDivider(clkDivider)         WS2812_DCLK_SetDividerRegister(clkDivider, 1u)
#define WS2812_DCLK_SetDividerValue(clkDivider)    WS2812_DCLK_SetDividerRegister((clkDivider) - 1u, 1u)
#define WS2812_DCLK_SetMode(clkMode)               WS2812_DCLK_SetModeRegister(clkMode)
#define WS2812_DCLK_SetSource(clkSource)           WS2812_DCLK_SetSourceRegister(clkSource)
#if defined(WS2812_DCLK__CFG3)
#define WS2812_DCLK_SetPhase(clkPhase)             WS2812_DCLK_SetPhaseRegister(clkPhase)
#define WS2812_DCLK_SetPhaseValue(clkPhase)        WS2812_DCLK_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(WS2812_DCLK__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define WS2812_DCLK_CLKEN              (* (reg8 *) WS2812_DCLK__PM_ACT_CFG)
#define WS2812_DCLK_CLKEN_PTR          ((reg8 *) WS2812_DCLK__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define WS2812_DCLK_CLKSTBY            (* (reg8 *) WS2812_DCLK__PM_STBY_CFG)
#define WS2812_DCLK_CLKSTBY_PTR        ((reg8 *) WS2812_DCLK__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define WS2812_DCLK_DIV_LSB            (* (reg8 *) WS2812_DCLK__CFG0)
#define WS2812_DCLK_DIV_LSB_PTR        ((reg8 *) WS2812_DCLK__CFG0)
#define WS2812_DCLK_DIV_PTR            ((reg16 *) WS2812_DCLK__CFG0)

/* Clock MSB divider configuration register. */
#define WS2812_DCLK_DIV_MSB            (* (reg8 *) WS2812_DCLK__CFG1)
#define WS2812_DCLK_DIV_MSB_PTR        ((reg8 *) WS2812_DCLK__CFG1)

/* Mode and source configuration register */
#define WS2812_DCLK_MOD_SRC            (* (reg8 *) WS2812_DCLK__CFG2)
#define WS2812_DCLK_MOD_SRC_PTR        ((reg8 *) WS2812_DCLK__CFG2)

#if defined(WS2812_DCLK__CFG3)
/* Analog clock phase configuration register */
#define WS2812_DCLK_PHASE              (* (reg8 *) WS2812_DCLK__CFG3)
#define WS2812_DCLK_PHASE_PTR          ((reg8 *) WS2812_DCLK__CFG3)
#endif /* defined(WS2812_DCLK__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define WS2812_DCLK_CLKEN_MASK         WS2812_DCLK__PM_ACT_MSK
#define WS2812_DCLK_CLKSTBY_MASK       WS2812_DCLK__PM_STBY_MSK

/* CFG2 field masks */
#define WS2812_DCLK_SRC_SEL_MSK        WS2812_DCLK__CFG2_SRC_SEL_MASK
#define WS2812_DCLK_MODE_MASK          (~(WS2812_DCLK_SRC_SEL_MSK))

#if defined(WS2812_DCLK__CFG3)
/* CFG3 phase mask */
#define WS2812_DCLK_PHASE_MASK         WS2812_DCLK__CFG3_PHASE_DLY_MASK
#endif /* defined(WS2812_DCLK__CFG3) */

#endif /* CY_CLOCK_WS2812_DCLK_H */


/* [] END OF FILE */
