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

void CyBtldrCommStart(void);
void CyBtldrCommStop (void);
void CyBtldrCommReset(void);

int main()
{
  CyGlobalIntEnable;
  LED_PWM_Start();
  
  //CySetTemp();
  Bootloader_Start();
  
  // Should never get here...
  for(;;){}


}

void CyBtldrCommStart(void)
{  
  
  USBUART_Start(0, USBUART_3V_OPERATION);
  while(USBUART_GetConfiguration() == 0)
  {  /* Await the completion of USB configuration */ }
  USBUART_CDC_Init();
  UART_Start();
}

void CyBtldrCommStop (void)
{
  USBUART_Stop();
  UART_Stop();
}

void CyBtldrCommReset(void)
{
  CyBtldrCommStop();
  CyBtldrCommStart();
}

/* [] END OF FILE */
