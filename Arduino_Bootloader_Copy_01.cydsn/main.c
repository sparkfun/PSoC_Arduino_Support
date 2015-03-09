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
#include "Bootloader.h"

void sendResponse(const uint8* buffer, uint16 len);
void writeToFlash(uint8* data, uint32 address, uint16 dataLen); 

int main()
{
  CyGlobalIntEnable; 
  
  // Start by assuming a bad application.
  cystatus *validApp;
  *validApp = CYRET_BAD_DATA;
  
  /* Bootloader_Start() is special for this application; does *not* match
   *  Cypress API documentation. */

  Bootloader_Start(validApp);
  
  Timer_Start();
  LED_PWM_Start();
  //timerInterrupt_StartEx((cyisraddress)0x00004000);
  USBUART_Start(0, USBUART_3V_OPERATION);
  while(USBUART_GetConfiguration() == 0)
  {  /* Await the completion of USB configuration */ }
  USBUART_CDC_Init();
  UART_Start();

  CySetTemp();

  static const uint8 deviceID[7] = {'A','V','R','B', 'O', 'O', 'T'};
  static const uint8 swID[2] = {'1', '0'};
  static const uint8 hwID[2] = {'1', '0'};
  uint16 bytesReceived = 0;
  uint8 inputBuffer[270];
  uint8 outputBuffer[256];
  uint32 currentAddress = 0x00000000;
  uint16 bytesToRead = 0;
  uint16 bytesExpected = 0;
  uint16 temp = 0;
  for(;;)
  {
    if (USBUART_DataIsReady())
    {
      bytesReceived = USBUART_GetAll(inputBuffer);
      if (bytesReceived > 0)
      {
        UART_PutArray(inputBuffer, (uint8)bytesReceived);
        switch(inputBuffer[0])
        {
          case 'A':
            while (bytesReceived < 3)
            {
              if ( USBUART_DataIsReady() )
              {
                bytesReceived += USBUART_GetAll(inputBuffer + bytesReceived);
              }
            }
            currentAddress = inputBuffer[1] << 9;
            currentAddress |= inputBuffer[2] << 1;
            outputBuffer[0] = '\r';
            sendResponse(outputBuffer, 1);
            break;
          case 'g':
            while (bytesReceived < 4)
            {
              if ( USBUART_DataIsReady() )
              {
                bytesReceived += USBUART_GetAll(inputBuffer + bytesReceived);
              }
            }
            bytesToRead = inputBuffer[1] << 8;
            bytesToRead |= inputBuffer[2];
            uint16 i = 0;
            for (i = 0; i<bytesToRead; ++i)
            {
              sendResponse((uint8*)(currentAddress + i), 1);
            }
            break;
          case 'B':
            /* When we enter here, we've received some number of bytes, up
             * to 64. If we've received *at least* 4, we have all the info
             * we need to proceed. */
            temp = bytesReceived;
            while (bytesReceived < 5)
            {
              /* If we've received less than 4, we need to collect data until
               * we have at least 4, so we have all we need to figure out how
               * many we're ultimately looking for. */
              if ( USBUART_DataIsReady() )
              {
                bytesReceived += USBUART_GetAll(inputBuffer + bytesReceived);
              }
            }
            bytesExpected = inputBuffer[1]<<8 | inputBuffer[2];
            while (bytesReceived < bytesExpected + 4)
            {
              if ( USBUART_DataIsReady() )
              {
                bytesReceived += USBUART_GetAll(inputBuffer + bytesReceived);
              }
            }
            UART_PutArray(inputBuffer, (uint8)(bytesReceived-temp));
            writeToFlash(inputBuffer+4, currentAddress, bytesExpected);
            currentAddress += 256;
            outputBuffer[0] = '\r';
            sendResponse(outputBuffer, 1);
            break;
          case 'S':
            sendResponse(deviceID, 8);
            break;
          case 'V':
            sendResponse(swID, 2);
            break;
          case 'p':
            outputBuffer[0] = 'S';
            sendResponse(outputBuffer, 1);
            break;
          case 'a':
            outputBuffer[0] = 'Y';
            sendResponse(outputBuffer, 1);
            break;
          case 'b':
            outputBuffer[0] = 'Y';
            outputBuffer[1] = 0x01;
            outputBuffer[2] = 0x00;
            sendResponse(outputBuffer, 3);
            break;
          case 'v':
            sendResponse(hwID, 2);
            break;
          case 't':
            outputBuffer[0] = 0x44;
            outputBuffer[1] = 0;
            sendResponse(outputBuffer, 2);
            break;
          case 's':
            outputBuffer[0] = 0x87;
            outputBuffer[1] = 0x95;
            outputBuffer[2] = 0x1e;
            sendResponse(outputBuffer, 3);
            break;
          case 'F':
          case 'N':
          case 'Q':
          case 'r':
            outputBuffer[0] = 0x00;
            sendResponse(outputBuffer, 1);
            break;
          case 'T':
          case 'x':
          case 'y':
          case 'P':
          case 'L':
            outputBuffer[0] = '\r';
            sendResponse(outputBuffer, 1);
            break;
          case 'E':
            outputBuffer[0] = '\r';
            sendResponse(outputBuffer, 1);
            Bootloader_Exit(Bootloader_EXIT_TO_BTLDB);
          default:
            UART_PutChar('?');
            while (USBUART_CDCIsReady() == 0)
            { /* wait for the CDC to be ready to ship out data */ }
            USBUART_PutChar('?');
        }
        bytesReceived = 0;
      }
    }
  }
}

void sendResponse(const uint8* buffer, uint16 len)
{
  while (USBUART_CDCIsReady() == 0)
  { /* wait for the CDC to be ready to ship out data */ }
  USBUART_PutData(buffer, len);
  UART_PutArray(buffer, len);
}

void writeToFlash(uint8* data, uint32 address, uint16 dataLen) 
{
  const uint32 flsRowSize = 0x000000100;
  const uint32 flsSectorSize = 0x00010000;

  uint8 arrayId = ( uint8 )(address / flsSectorSize);

  uint16 rowNum = ( uint16 )((address % flsSectorSize) / flsRowSize);

  if (dataLen < flsRowSize)
  {
    uint8 memBuffer[256];
    memcpy(memBuffer, (const void*) address, 256);
    memcpy(memBuffer, data, dataLen);
  }

  CyWriteRowFull(arrayId, rowNum, data, flsRowSize); 

  /***************************************************************************
  * * When writing Flash, data in the instruction cache can become stale.
  * * Therefore, the cache data does not correlate to the data just written to
  * * Flash. A call to CyFlushCache() is required to invalidate the data in the
  * * cache and force fresh information to be loaded from Flash.
  * **************************************************************************/

  CyFlushCache();

}
/* [] END OF FILE */

