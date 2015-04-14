#include <Project.h>
#include <Arduino_Pins.h>
#include <Arduino_Extended_IO.h>

void (*extendedPinWrite[NUM_EXTENDED_PINS])(uint8_t level);
uint8_t (*extendedPinRead[NUM_EXTENDED_PINS])();
void (*extendedPinMode[NUM_EXTENDED_PINS])(uint8_t driveMode);


void initExtendedPins()
{
  extendedPinWrite[0] = P0_0_Write;
  extendedPinWrite[1] = P0_1_Write;
  extendedPinWrite[2] = P0_2_Write;
  extendedPinWrite[3] = P0_3_Write;
  extendedPinWrite[4] = P0_4_Write;
  extendedPinWrite[5] = P0_5_Write;
  extendedPinWrite[6] = P0_6_Write;
  extendedPinWrite[7] = P0_7_Write;
  extendedPinWrite[8] = P3_0_Write;
  extendedPinWrite[9] = P3_1_Write;
  extendedPinWrite[10] = P3_2_Write;
  extendedPinWrite[11] = P3_3_Write;
  extendedPinWrite[12] = P3_4_Write;
  extendedPinWrite[13] = P3_5_Write;
  extendedPinWrite[14] = P3_6_Write;
  extendedPinWrite[15] = P3_7_Write;
  extendedPinWrite[16] = P4_0_Write;
  extendedPinWrite[17] = P4_1_Write;
  extendedPinWrite[18] = P4_2_Write;
  extendedPinWrite[19] = P4_3_Write;
  extendedPinWrite[20] = P4_4_Write;
  extendedPinWrite[21] = P4_5_Write;
  extendedPinWrite[22] = P4_6_Write;
  extendedPinWrite[23] = P4_7_Write;
  extendedPinWrite[24] = P5_0_Write;
  extendedPinWrite[25] = P5_1_Write;
  extendedPinWrite[26] = P5_2_Write;
  extendedPinWrite[27] = P5_3_Write;
  extendedPinWrite[28] = P5_4_Write;
  extendedPinWrite[29] = P5_5_Write;
  extendedPinWrite[30] = P5_6_Write;
  extendedPinWrite[31] = P5_7_Write;
  extendedPinWrite[32] = P12_0_Write;
  extendedPinWrite[33] = P12_1_Write;
  extendedPinWrite[34] = P12_2_Write;
  extendedPinWrite[35] = P12_3_Write;
  extendedPinWrite[36] = P1_7_Write;
  extendedPinWrite[37] = P1_5_Write;
  extendedPinWrite[38] = P12_6_Write;
  extendedPinWrite[39] = P12_7_Write;

  extendedPinRead[0] = P0_0_Read;
  extendedPinRead[1] = P0_1_Read;
  extendedPinRead[2] = P0_2_Read;
  extendedPinRead[3] = P0_3_Read;
  extendedPinRead[4] = P0_4_Read;
  extendedPinRead[5] = P0_5_Read;
  extendedPinRead[6] = P0_6_Read;
  extendedPinRead[7] = P0_7_Read;
  extendedPinRead[8] = P3_0_Read;
  extendedPinRead[9] = P3_1_Read;
  extendedPinRead[10] = P3_2_Read;
  extendedPinRead[11] = P3_3_Read;
  extendedPinRead[12] = P3_4_Read;
  extendedPinRead[13] = P3_5_Read;
  extendedPinRead[14] = P3_6_Read;
  extendedPinRead[15] = P3_7_Read;
  extendedPinRead[16] = P4_0_Read;
  extendedPinRead[17] = P4_1_Read;
  extendedPinRead[18] = P4_2_Read;
  extendedPinRead[19] = P4_3_Read;
  extendedPinRead[20] = P4_4_Read;
  extendedPinRead[21] = P4_5_Read;
  extendedPinRead[22] = P4_6_Read;
  extendedPinRead[23] = P4_7_Read;
  extendedPinRead[24] = P5_0_Read;
  extendedPinRead[25] = P5_1_Read;
  extendedPinRead[26] = P5_2_Read;
  extendedPinRead[27] = P5_3_Read;
  extendedPinRead[28] = P5_4_Read;
  extendedPinRead[29] = P5_5_Read;
  extendedPinRead[30] = P5_6_Read;
  extendedPinRead[31] = P5_7_Read;
  extendedPinRead[32] = P12_0_Read;
  extendedPinRead[33] = P12_1_Read;
  extendedPinRead[34] = P12_2_Read;
  extendedPinRead[35] = P12_3_Read;
  extendedPinRead[36] = P1_7_Read;
  extendedPinRead[37] = P1_5_Read;
  extendedPinRead[38] = P12_6_Read;
  extendedPinRead[39] = P12_7_Read;

  extendedPinMode[0] = P0_0_Mode;
  extendedPinMode[1] = P0_1_Mode;
  extendedPinMode[2] = P0_2_Mode;
  extendedPinMode[3] = P0_3_Mode;
  extendedPinMode[4] = P0_4_Mode;
  extendedPinMode[5] = P0_5_Mode;
  extendedPinMode[6] = P0_6_Mode;
  extendedPinMode[7] = P0_7_Mode;
  extendedPinMode[8] = P3_0_Mode;
  extendedPinMode[9] = P3_1_Mode;
  extendedPinMode[10] = P3_2_Mode;
  extendedPinMode[11] = P3_3_Mode;
  extendedPinMode[12] = P3_4_Mode;
  extendedPinMode[13] = P3_5_Mode;
  extendedPinMode[14] = P3_6_Mode;
  extendedPinMode[15] = P3_7_Mode;
  extendedPinMode[16] = P4_0_Mode;
  extendedPinMode[17] = P4_1_Mode;
  extendedPinMode[18] = P4_2_Mode;
  extendedPinMode[19] = P4_3_Mode;
  extendedPinMode[20] = P4_4_Mode;
  extendedPinMode[21] = P4_5_Mode;
  extendedPinMode[22] = P4_6_Mode;
  extendedPinMode[23] = P4_7_Mode;
  extendedPinMode[24] = P5_0_Mode;
  extendedPinMode[25] = P5_1_Mode;
  extendedPinMode[26] = P5_2_Mode;
  extendedPinMode[27] = P5_3_Mode;
  extendedPinMode[28] = P5_4_Mode;
  extendedPinMode[29] = P5_5_Mode;
  extendedPinMode[30] = P5_6_Mode;
  extendedPinMode[31] = P5_7_Mode;
  extendedPinMode[32] = P12_0_Mode;
  extendedPinMode[33] = P12_1_Mode;
  extendedPinMode[34] = P12_2_Mode;
  extendedPinMode[35] = P12_3_Mode;
  extendedPinMode[36] = P1_7_Mode;
  extendedPinMode[37] = P1_5_Mode;
  extendedPinMode[38] = P12_6_Mode;
  extendedPinMode[39] = P12_7_Mode;

  uint8_t i = 0;
  for (i; i< NUM_EXTENDED_PINS; i++)
  {
    pinMode(i+NUM_BASE_PINS, INPUT);
  }
}
