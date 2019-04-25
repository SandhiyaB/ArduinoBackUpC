#include "fingerprint.h"

void setup() 
{
  fingerprint_setup();
}

void loop() 
{
  // Create a new fingerprint entry
  enrollFingerprint(0x01);
  delay(1000);

  // Request entry
  Serial.println(" \nUSER LOGIN REQUEST...PLACE FINGER ONTO SENSOR  \n");
  while(readFingerprint() == -1);
  Serial.println(" \nACCESS GRANTED \n");
  Serial.println(" \nFingerprint confidence : " + String(confidence) + " \n");  
  delay(3000);
}
