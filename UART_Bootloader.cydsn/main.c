#include <device.h>

/* The clock input to the ResetCounter timer is 24MHz. By default, we want a
*  10-second timeout before dumping the bootloader and going to the app. */
#define TIMEOUT_PERIOD 240
//#define USE_UART
int main()
{
  CyGlobalIntEnable;
  
  //char testString[128];
  //sprintf(testString, "I am the very model of a modern major general");

	/* Initialize PWM */
  PWM_Start();  
  ResetCounter_Start();
  ResetCounter_WriteCompare(TIMEOUT_PERIOD);
  #ifdef USE_UART
  UART_Start();
  #endif
  
  //UART_PutString(testString);
	
	/* This API does the entire bootload operation. After a succesful bootload 
  *   operation, this API transfers program control to the new application via 
  *   a software reset */
	Bootloader_Start();
	
	/* CyBtldr_Start() API does not return – it ends with a software device reset. So, the code 
	   after this API call (below) is never executed. */
  for(;;)
  {
      
  }
}

void CyBtldrCommStart(void)
{  
  USBUART_Start(0, USBUART_3V_OPERATION);
  while(USBUART_GetConfiguration() == 0)
  {  /* Await the completion of USB configuration */ }
  USBUART_CDC_Init();
}

void CyBtldrCommStop (void)
{
  USBUART_Stop();
}

void CyBtldrCommReset(void)
{
  USBUART_CyBtldrCommReset();
}

cystatus CyBtldrCommWrite(uint8* buffer, uint16 size, uint16* count, uint8 timeOut)
{
  cystatus retVal = CYRET_TIMEOUT;
  
  *count = 0;
  uint16 internalTimeout = (timeOut*10) + 100;
  uint8 packetsSent = 0;
  uint8 numPackets = size/64;
  uint8 lastPacketSize = size%64;
  /* The sending loop. There are two limits on this:
   1. We can't send data until the prior data transmission is done, so we have
      to wait until USBUART_CDCIsReady() returns a non-zero value.
   2. We can only send 64 bytes at a go.
  */
  while (internalTimeout > 0)
  {
    if (USBUART_CDCIsReady() == 0)
    {
      CyDelay(1);
      --internalTimeout;
      continue;
    }
    /* Once here, we can send out our packets one at a time. There are three
       cases:
     1. More than 64 bytes to send, so do numPackets of 64 bytes followed by one
        packet of lastPacketSize bytes.
     2. 64 bytes to send, so do one packet of 64 bytes, then one packet of zero.
     3. Less than 64 bytes, so do one packet of lastPacketSize bytes.
    */
    else
    {
      if (numPackets > 0)
      {
        USBUART_PutData((buffer+(packetsSent*64)), 64);
        #ifdef USE_UART
        while(UART_GetTxBufferSize() != 0)
        { /* wait for clear data path on UART */ }
        UART_PutArray((buffer+(packetsSent*64)), 64);
        #endif
        ++packetsSent;
        --numPackets;
        *count += 64;
      }
      else if (numPackets == 0)
      {
        USBUART_PutData((buffer+(packetsSent*64)), lastPacketSize);
        #ifdef USE_UART
        while(UART_GetTxBufferSize() != 0)
        { /* wait for clear data path on UART */ }
        UART_PutArray((buffer+(packetsSent*64)), lastPacketSize);
        #endif
        *count += lastPacketSize;
        retVal = CYRET_SUCCESS;
        break;
      }
    }
  }
  return retVal;
}

cystatus CyBtldrCommRead (uint8* buffer, uint16 size, uint16* count, uint8 timeOut)
{
  cystatus retVal = CYRET_TIMEOUT;
  if (timeOut) timeOut = 0;
  
  *count = 0;
  uint16 dataAvailableCnt = 0;
  uint16 internalTimeout = 10;
  while (internalTimeout > 0)
  {
    if (USBUART_DataIsReady() == 0)
    {
      CyDelay(1);
      --internalTimeout;
      continue;
    }
    else
    {
      
      dataAvailableCnt = USBUART_GetCount();
      if (dataAvailableCnt > 0) 
      {
      internalTimeout = 2;
      }
      USBUART_GetData((buffer + *count), dataAvailableCnt);
      #ifdef USE_UART
      while(UART_GetTxBufferSize() != 0)
      { /* wait for clear data path on UART */ }
      UART_PutArray((buffer+*count), dataAvailableCnt);
      #endif
      *count += dataAvailableCnt;
      if (*count > 0)
      {
        retVal = CYRET_SUCCESS;
      }
      if (*count >= (size - 1))
      {
        break;
      }
      if (internalTimeout == 0)
      {
        break;
      }
    }
  }
  return retVal;
}
/* [] END OF FILE */
