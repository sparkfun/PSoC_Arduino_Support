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

#include "Project.h"

#ifndef _arduino_i2c_h
#define _arduino_i2c_h
    
  enum XFER_MODE {COMPLETE, REPEATED_START, NO_STOP};

class Wire
{
  public:
    void begin();
    uint8_t requestFrom(uint8_t address, uint8_t quantity, \
        enum XFER_MODE stop);
    void beginTransmission(uint8_t address);
    uint8_t endTransmission(enum XFER_MODE stop);
    uint8_t write(uint8_t *buffer, uint8_t length);
    uint8_t available();
    uint8_t read();
}


#endif

/* [] END OF FILE */
