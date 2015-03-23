extern "C" {
#include <project.h>
}
#include "Arduino.h"
#include "Arduino_Serial.h"
#include "core/UARTClass.h"

/*******************************************************
 * This file includes the middle-man functions which lie between the UART
 * class and the Cypress API.
 * We'll add to it as needed to support additional hardware in other virtual
 * boards.
 ******************************************************/

extern struct SerialHelperFuncs USBUART_Helpers;
extern struct SerialHelperFuncs Serial1_Helpers;

// Wait for PC to configure USB for CONFIG_DELAY * CONFIG_LOOPS milliseconds
#define CONFIG_DELAY 10
#define CONFIG_LOOPS 200

bool enableUSBCDC()
{
  USBUART_Start(0, USBUART_3V_OPERATION);
  uint8_t delayCounter = 0;
  /* It's important that we not loop forever here, or we'll never leave in
   * cases where the device isn't plugged into a PC. We do, however, want to
   * provide enough time for the PC to do its thing. */  
  while(USBUART_GetConfiguration() == 0)
  {  
    CyDelay(10);
    delayCounter++;
    if (delayCounter > 200)
    {
      return false;
    }
  }
  USBUART_CDC_Init();
  return true;
}

/**********************************************************************
 * FunctionAttach() functions should point these to some implementation
 * for the particular interface. Not all of them have to be implemented
 * for all types of interface, as noted.
  uint16_t (*bufferSize)(void); - Return number of bytes in the peripheral
    buffer.
  void (*sendData)(const uint8_t* buffer, uint16_t size);
  uint16_t (*getData)(uint8_t* buffer, uint16_t size);
  bool (*blockForReadyToWrite)(void); 

These are for non-USB UARTs only:
  void (*clockAdjust)(uint16_t newDivider); - Changes the clock divider on
    the UART clock input to provide the new baudrate output.
  bool (*portEnable)(void);
  void (*portDisable)(void);
  void (*blockForWriteComplete)(void);
**********************************************************************/
void USBUART_FunctionAttach()
{
  USBUART_Helpers.bufferSize = USBUART_GetCount;
  USBUART_Helpers.sendData = USBUART_PutData;
  USBUART_Helpers.getData = USBUART_GetData;
  USBUART_Helpers.blockForReadyToWrite = USBUART_CDCIsReady;
}

uint16_t Serial1_CustomGetDataArray(uint8_t* buffer, uint16_t size);
uint16_t Serial1_CustomGetBufferSize(void);
void     Serial1_CustomPutArray(const uint8_t* buffer, uint16_t size);
bool     Serial1_CustomPortEnable(void);
void     Serial1_CustomPortDisable(void);
void     UART1Clock_CustomSetDivider(uint16_t newDivider);

void Serial1_FunctionAttach()
{
  Serial1_Helpers.bufferSize = Serial1_CustomGetBufferSize;
  Serial1_Helpers.sendData = Serial1_CustomPutArray;
  Serial1_Helpers.getData = Serial1_CustomGetDataArray;
  Serial1_Helpers.portEnable = Serial1_CustomPortEnable;
  Serial1_Helpers.portDisable = Serial1_CustomPortDisable;
  Serial1_Helpers.clockAdjust = UART1Clock_CustomSetDivider;
}


/**********************************************************************
 * Sometimes, it's not possible to make a 1:1 conversion between the needs
 * of the UART class and the available functions in the Cypress API. Below
 * are the functions which implement that conversion.
 * *******************************************************************/

typedef uint8_t (*UART_getDataFunc)(void);
typedef uint8_t (*UART_getBufferSize)(void);

UART_getDataFunc readByte;
UART_getBufferSize readBufSize;

uint16_t UART_GetDataArray(uint8_t* buffer, uint8_t size);

uint16_t Serial1_CustomGetDataArray(uint8_t* buffer, uint16_t size)
{
  readByte = Serial1_ReadRxData;
  readBufSize = Serial1_GetRxBufferSize;
  return UART_GetDataArray(buffer, size);
}

uint16_t Serial1_CustomGetBufferSize(void)
{
  return (uint16_t)Serial1_GetRxBufferSize();
}

void Serial1_CustomPutArray(const uint8_t* buffer, uint16_t size)
{
  Serial1_PutArray(buffer, size);
}

bool Serial1_CustomPortEnable(void)
{
  Serial1_Start();
  D0_Bypass(PIN_ENABLE_BYPASS);
  pinMode(D1, OUTPUT);
  D1_Bypass(PIN_ENABLE_BYPASS);
  return true; 
}

void Serial1_CustomPortDisable(void)
{
  Serial1_Stop();
  D0_Bypass(PIN_DISABLE_BYPASS);
  D1_Bypass(PIN_DISABLE_BYPASS); 
}

uint16_t UART_GetDataArray(uint8_t* buffer, uint8_t size)
{
  uint16_t bytesRead = 0;
  for (int i = 0; i < size; ++i)
  {
    if (readBufSize() > 0)
    {
      buffer[i] = readByte();
      ++bytesRead;
    }
    else
    {
      break;
    }
  }
  return bytesRead;
}

/***************************************************************************
 * WHY!?!?!
 * Why have a function which calls a seemingly identical function? It's not
 * even abstracted or anything?!?!?!
 * Because, SetDividerValue isn't a function, it's a macro for a function
 * which is incompatible, so trying to use a pointer that's compatible with
 * SetDividerValue doesn't work. Blech.
 * ***********************************************************************/

void UART1Clock_CustomSetDivider(uint16_t newDivider)
{
  //UART1Clock_SetDividerValue(newDivider);
}

