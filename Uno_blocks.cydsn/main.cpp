extern "C" {

#include <project.h>

}

#include "Arduino.h"

#define I2C      0
#define ANALOG   1
#define DIGITAL  2

struct SerialHelperFuncs USBUART_Helpers;
struct SerialHelperFuncs Serial1_Helpers;
UARTClass Serial(true, &USBUART_Helpers);
UARTClass Serial1(false, &Serial1_Helpers);

void exitToBootloader(void);

int main()
{
    pinFuncInit();
    
    CyGlobalIntEnable;   
    USBUART_FunctionAttach();
    //enableUSBCDC();
    Serial1_FunctionAttach();
    
    setup();
      
    for(;;)
    {
      loop();
      if (Serial)
        {
        if (USBUART_IsLineChanged())
        {
          if (USBUART_GetDTERate() == 1200)
          {
            exitToBootloader();
          }
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

