extern "C" {
#include <project.h>
}

#include "Arduino.h"

#define Bootloader_SCHEDULE_BTLDR   (0x40u)
#define Bootloader_SCHEDULE_BTLDB   (0x80u)
#define Bootloader_RESET_SR0_REG    (* (reg8 *) CYREG_RESET_SR0)
#define Bootloader_SET_RUN_TYPE(x)  (Bootloader_RESET_SR0_REG = (x))

#define I2C      0
#define ANALOG   1
#define DIGITAL  2

struct SerialHelperFuncs USBUART_Helpers;
struct SerialHelperFuncs UART_Helpers;
UARTClass Serial(true, &USBUART_Helpers);
UARTClass Serial1(false, &UART_Helpers);

boolean USB_IS_Available;

void exitToBootloader(void);

int main()
{
    
  CyGlobalIntEnable;  
  Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
  USBUART_FunctionAttach();
  //enableUSBCDC();
  UART_FunctionAttach();
  initTimebase();
  
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


void exitToBootloader(void)
{
  Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDR);
  CySoftwareReset();
}


/* [] END OF FILE */

