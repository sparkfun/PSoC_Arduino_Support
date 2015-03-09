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

#include <project.h>
#include <stdio.h>
#include "Arduino_Pins.h"
#include "Arduino_I2C.h"

#define I2C      0
#define ANALOG   1
#define DIGITAL  2

int main()
{
    pinFuncInit();
    
    CyGlobalIntEnable; 
    
    uint8_t mode = DIGITAL;

    if (mode == DIGITAL)
    {
        pinMode(A4, INPUT_PULLUP);
        pinMode(A5, OUTPUT);
    }
    else if (mode == ANALOG)
    {
    }
    else
    {
        I2C_begin();
    }
    
    for(;;)
    {
        if (mode == DIGITAL)
        {
            if (digitalRead(A4) == LOW)
            {
                digitalWrite(A5, HIGH);
                CyDelay(500);
            }
            else digitalWrite(A5, LOW);
        }
        else if (mode == ANALOG)
        {
            ADC_StartConvert();
            ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
            char strTemp[15];
            uint16_t ADCVoltage = ADC_GetResult16();
            float fVoltage = ADC_CountsTo_Volts(ADCVoltage);
            sprintf(strTemp,"%f", fVoltage);
        }
        else
        {
            I2C_beginTransmission(0x68);
            uint8_t temp = 0x75;
            I2C_write(&temp, 1);
            I2C_endTransmission(NO_STOP);
            I2C_requestFrom(0x68, 1, REPEATED_START);
            uint8_t rxTemp = I2C_read();
            char strTemp[5];
            sprintf(strTemp, "%x", rxTemp);
            CyDelay(500);
        }
    }
}

/* [] END OF FILE */

