/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef _arduino_pins_h
#define _arduino_pins_h
  
#include <stdint.h>
#include <Project.h>
    
    #define D0 0
    #define D1 1
    #define D2 2
    #define D3 3
    #define D4 4
    #define D5 5
    #define D6 6
    #define D7 7
    #define D8 8
    #define D9 9
    #define D10 10
    #define D11 11
    #define D12 12
    #define D13 13
    #define D14 14
    #define D15 15
    #define D16 16
    #define D17 17
    #define D18 18
    #define D19 19
    #define A0 14
    #define A1 15
    #define A2 16
    #define A3 17
    #define A4 18
    #define A5 19
    
    #define HIGH 1u
    #define LOW 0u
    
    enum PIN_MODE {OUTPUT, INPUT, INPUT_PULLUP, PERIPHERAL};
    enum BYPASS_MODE {PIN_ENABLE_BYPASS, PIN_DISABLE_BYPASS};
    
    void pinFuncInit(void);
    void digitalWrite(uint8_t pin, uint8_t level);
    void pinMode(uint8_t pin, enum PIN_MODE mode);
    uint8_t digitalRead(uint8_t pin);
    void D0_Bypass(enum BYPASS_MODE enableBypass);
    void D1_Bypass(enum BYPASS_MODE enableBypass);
    void D2_Bypass(enum BYPASS_MODE enableBypass);
    void D3_Bypass(enum BYPASS_MODE enableBypass);
    void D4_Bypass(enum BYPASS_MODE enableBypass);
    void D5_Bypass(enum BYPASS_MODE enableBypass);
    void D6_Bypass(enum BYPASS_MODE enableBypass);
    void D7_Bypass(enum BYPASS_MODE enableBypass);
    void D8_Bypass(enum BYPASS_MODE enableBypass);
    void D9_Bypass(enum BYPASS_MODE enableBypass);
    void D10_Bypass(enum BYPASS_MODE enableBypass);
    void D11_Bypass(enum BYPASS_MODE enableBypass);
    void D12_Bypass(enum BYPASS_MODE enableBypass);
    void D13_Bypass(enum BYPASS_MODE enableBypass);
    void A0_Bypass(enum BYPASS_MODE enableBypass);
    void A1_Bypass(enum BYPASS_MODE enableBypass);
    void A2_Bypass(enum BYPASS_MODE enableBypass);
    void A3_Bypass(enum BYPASS_MODE enableBypass);
    void A4_Bypass(enum BYPASS_MODE enableBypass);
    void A5_Bypass(enum BYPASS_MODE enableBypass);
    
#endif

/* [] END OF FILE */
