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

#include <stdio.h>
#include "Arduino.h"
#include "Arduino_Pins.h"
#include "Arduino_I2C.h"

#define I2C      0
#define ANALOG   1
#define DIGITAL  2

int main()
{
    pinFuncInit();
    
    CyGlobalIntEnable;   
    USBUART_Start(0, USBUART_3V_OPERATION);
    while(USBUART_GetConfiguration() == 0)
    {  /* Await the completion of USB configuration */ }
    USBUART_CDC_Init();
    
    setup();
      
    for(;;)
    {
      loop();

    }
}

/* [] END OF FILE */

