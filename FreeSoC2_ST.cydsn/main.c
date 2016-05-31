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

//DO NOT FORGET TO CHANGE THE DEVICE WHEN GENERATING HEX!

#include <project.h>
#include <cypins.h>
#include <stdio.h>

// Needed to allow the ST firmware to respond to requests from Arduino to reset
//  into the bootloader.
static void exitToBootloader(void);
CY_ISR_PROTO(BootloaderReset_ISR);

#define Bootloader_SCHEDULE_BTLDR   (0x40u)
#define Bootloader_SCHEDULE_BTLDB   (0x80u)
#define Bootloader_RESET_SR0_REG    (* (reg8 *) CYREG_RESET_SR0)
#define Bootloader_SET_RUN_TYPE(x)  (Bootloader_RESET_SR0_REG = (x))

uint8 en_1 = 0;
uint8 en_2 = 1;
uint8 net_1_ON_val = 0;
uint8 net_2_ON_val = 0;
uint8 net_1_OFF_val = 0;
uint8 net_2_OFF_val = 0;
uint8 ST_result = 0;

int main()
{   
    CyGlobalIntEnable; 
    BootloaderResetTimer_Start();
    BootloaderResetInterrupt_StartEx(BootloaderReset_ISR);
    Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDB);
    en_1 = 0;
    en_2 = 1;
    net_1_ON_val = 0;
    net_2_ON_val = 0;
    net_1_OFF_val = 0;
    net_2_OFF_val = 0;
    ST_result = 0;
    PASS_LED_Write(0);
    
     en_1 = ST_EN_HIGH_Read();
     en_2 = ST_EN_LOW_Read();
    
   if((en_1 != 0) && (en_2 == 0))
    {
    
    
        net_1_ON_val = 0;
        net_2_ON_val = 0;
        
        NET_1_CTRL_Write(0);
        NET_2_CTRL_Write(1);
        CyDelay(500);
    
    if(Pin_1_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_2_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_3_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_4_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_5_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_6_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_7_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_8_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_9_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_10_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_11_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_12_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_13_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_14_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_15_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_16_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_17_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_18_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_19_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_20_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_21_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_22_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_23_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_24_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_25_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_26_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_27_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_28_Read() == 0)
    {
        net_1_ON_val += 1;
    }
     if(Pin_29_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    if(Pin_30_Read() == 0)
    {
        net_1_ON_val += 1;
    }
    //END of NET_1 HIGH NET_2 LOW Section
    
    
    NET_1_CTRL_Write(1);
    NET_2_CTRL_Write(0);
    
    if((Pin_31_Read() == 0) && (Pin_1_Read() != 0))
    {
        net_2_ON_val += 1;
        
    }
    if(Pin_32_Read() == 0)
    {
       net_2_ON_val += 1;
        
    }
    if(Pin_33_Read() == 0)
    {
       net_2_ON_val += 1;
        
    }
      if(Pin_34_Read() == 0)
    {
       net_2_ON_val += 1;
        
    }
      if(Pin_35_Read() == 0)
    {
      net_2_ON_val += 1;
        
    }
      if(Pin_36_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_37_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_38_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_39_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_40_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_41_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_42_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_43_Read() == 0)
    {
       net_2_ON_val += 1;
        
    }
      if(Pin_44_Read() == 0)
    {
       net_2_ON_val += 1;
        
    }
      if(Pin_46_Read() == 0)
    {
       net_2_ON_val += 1;
        
    }
      if(Pin_47_Read() == 0)
    {
       net_2_ON_val += 1;
        
    }
      if(Pin_48_Read() == 0)
    {
       net_2_ON_val += 1;
        
    }
      if(Pin_49_Read() == 0)
    {
        net_2_ON_val += 1;
       
    }
      if(Pin_50_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_51_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_52_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_53_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_54_Read() == 0)
    {
        net_2_ON_val += 1;
       
    }
      if(Pin_55_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_56_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_57_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
      if(Pin_58_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
     if(Pin_45_Read() == 0)
    {
        net_2_ON_val += 1;
        
    }
    //END of NET_1 HIGH NET_2 LOW Section
    
    NET_1_CTRL_Write(0);
    NET_2_CTRL_Write(0);
    
    if((net_1_ON_val == 30) && (net_2_ON_val == 28))
    {
       PASS_LED_Write(1);
    }
    }
    else
    {
       /* Place your initialization/startup code here (e.g. MyInst_Start()) */ 
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
    USBUART_Start(0, USBUART_3V_OPERATION);
    while(USBUART_GetConfiguration() == 0)
    {  /* Await the completion of USB configuration */ }
    USBUART_CDC_Init();  
        
    char net1Result[64];
    sprintf(net1Result, "Pass code NET_1: %x\n\r", (net_1_ON_val)); 
    USBUART_PutString(net1Result);
    CyDelay(1500);
    
    char net2Result[64];
    sprintf(net2Result, "Pass code NET_2: %x\n\r", (net_2_ON_val)); 
    USBUART_PutString(net2Result);
    CyDelay(1500);
    }
        
        //char Hello[64];
        //sprintf(Hello, "Hello: %x\n\r", (net_1_ON_val)); 
        //USBUART_PutString(Hello);
        //CyDelay(1500);
     for(;;)
    {}
}

CY_ISR(BootloaderReset_ISR)
{
  BootloaderResetInterrupt_ClearPending();         
  if(USBUART_GetConfiguration() != 0u)
  { 
  }
  if (USBUART_GetDTERate() == 1200)
  {
    exitToBootloader();
  }
}

void exitToBootloader(void)
{
  Bootloader_SET_RUN_TYPE(Bootloader_SCHEDULE_BTLDR);
  CySoftwareReset();
}