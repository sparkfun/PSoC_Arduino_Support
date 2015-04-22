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
    PWM_1_Start();
    PWM_2_Start();
    PWM_3_Start();
    D10_Ctrl_Write(0x00);
    D11_Ctrl_Write(0x00);
}

void analogWrite(uint32_t pin, uint16_t value)
{
  switch (pin)
  {
    case 3:
      pin = D3;
      break;
    case 5:
      pin = D5;
      break;
    case 6:
      pin = D6;
      break;
    case 9:
      pin = D9;
      break;
    case 10:
      pin = D10;
      break;
    case 11:
      pin = D11;
      break;
    default:
      break;
  }
      
  switch(pin)
  {
    case D3:
      PWM_1_WriteCompare1(value);
      break;
    case D5:
      PWM_1_WriteCompare2(value);
      break;
    case D6:
      PWM_2_WriteCompare1(value);
      break;
    case D9: 
      PWM_2_WriteCompare2(value);
      break;
    case D10:
      PWM_3_WriteCompare1(value);
      D10_Ctrl_Write(0x00);
      break;
    case D11:
      PWM_3_WriteCompare2(value);
      D11_Ctrl_Write(0x00);
      break;
    default:
      return;
  }
  pinMode(pin, PERIPHERAL);
}
/* [] END OF FILE */
