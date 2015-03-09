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
#include <Project.h>
}
#include <Arduino_Time.h>

void initTimebase(void)
{
    Clock_2_Start();
    Clock_3_Start();
    Micros_Counter_Start();
    Millis_Counter_Start();
}

void delay(unsigned int delayms)
{
  CyDelay(delayms);
}

void delayMicroseconds(unsigned int delayus)
{
  CyDelayUs(delayus);
}

/* [] END OF FILE */
