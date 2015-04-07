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

#include <Project.h>
#include <Arduino_Pins.h>
#include <Arduino_Analog_Out.h>

void analogWriteInit()
{
    PWM_1_Start();
    PWM_2_Start();
    PWM_3_Start();
    D10_Ctrl_Write(0x00);
    D11_Ctrl_Write(0x00);
}

void analogWrite(uint8_t pin, uint16_t value)
{
    switch(pin)
    {
        case D3:
        D3_BYP |= D3_MASK;
        PWM_1_WriteCompare1(value);
        break;
        case D5:
        D5_BYP |= D5_MASK;
        PWM_1_WriteCompare2(value);
        break;
        case D6:
        D6_BYP |= D6_MASK;
        PWM_2_WriteCompare1(value);
        break;
        case D9:
        D9_BYP |= D9_MASK;
        PWM_2_WriteCompare2(value);
        break;
        case D10:
        D10_BYP |= D10_MASK;
        PWM_3_WriteCompare1(value);
        D10_Ctrl_Write(0x00);
        break;
        case D11:
        D11_BYP |= D11_MASK;
        PWM_3_WriteCompare2(value);
        D11_Ctrl_Write(0x00);
        break;
        default:
        return;
    }
}
/* [] END OF FILE */
