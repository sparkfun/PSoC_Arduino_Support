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
extern "C" {
#include <project.h>
}

#include <Arduino_Pins.h>
#include <Arduino_Analog_Out.h>

void analogWriteInit()
{
  D10_Ctrl_Write(0x00);
  D11_Ctrl_Write(0x00);
}

void analogWrite(uint32_t pin, uint8_t value)
{
  switch (pin)
  {
    case 3:
      pin = D3;
      PWM_1_WriteCompare1(value);
      break;
    case 5:
      pin = D5;
      PWM_1_WriteCompare2(value);
      break;
    case 6:
      pin = D6;
      PWM_2_WriteCompare1(value);
      break;
    case 9:
      pin = D9;
      PWM_2_WriteCompare2(value);
      break;
    case 10:
      pin = D10;
      PWM_3_WriteCompare1(value);
      D10_Ctrl_Write(0x00);
      break;
    case 11:
      pin = D11;
      PWM_3_WriteCompare2(value);
      D11_Ctrl_Write(0x00);
      break;
    default:
      return;
  }
      
  pinMode(pin, PERIPHERAL_OUT);
}
/* [] END OF FILE */

