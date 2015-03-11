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

void exitToBootloader(void);

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
      if (USBUART_IsLineChanged())
      {
        if (USBUART_GetDTERate() == 1200)
        {
          exitToBootloader();
        }
      }
    }
}

#define Bootloader_SCHEDULE_BTLDR   (0x00u)
#define Bootloader_RESET_SR0_REG    (* (reg8 *) CYREG_RESET_SR0)
#define Bootloader_SET_RUN_TYPE(x)  (Bootloader_RESET_SR0_REG = (x))

void exitToBootloader(void)
{
  Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDR);
  CySoftwareReset();
}

/* [] END OF FILE */

