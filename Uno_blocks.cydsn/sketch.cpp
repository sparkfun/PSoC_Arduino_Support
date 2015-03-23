#include "Arduino.h"

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(1, PERIPHERAL);
  Serial1.begin(9600);
  Serial1.println(F("temp"));
  Serial1.print("DOOD");
}

void loop()
{
  int bugz = 412;
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  Serial1.println("DOOD");
  Serial1.println(100);
  Serial1.println(bugz);
  Serial.println("NOWAY");
}

/* [] END OF FILE */
