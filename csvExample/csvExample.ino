#include "SD.h"
#include"SPI.h"
#include <ArduinoJson.h>
//
//the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD
// library functions will not work.
const int CSpin = 15;
String dataString =""; // holds the data to be written to the SD card
float sensorReading1 = 0.00; // value read from your first sensor
float sensorReading2 = 0.00; // value read from your second sensor
float sensorReading3 = 0.00; // value read from your third sensor
File sensorData;
//
//
StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();
int i=0;
void setup()
{
// Open serial communications
Serial.begin(9600);
Serial.print("Initializing SD card...");
pinMode(CSpin, OUTPUT);
//
// see if the card is present and can be initialized:
if (!SD.begin(CSpin)) {
Serial.println("Card failed, or not present");
// don't do anything more:
return;
}
Serial.println("card initialized.");
}
//
void loop(){
// build the data string

dataString = i; // convert to CSV
i=i+1;
readData(); // save to SD card
//saveData();
delay(1000); // delay before next write to SD Card, adjust as required
}

void readData(){
if(SD.exists("json.csv")){ // check the card is still there
// now append new data filewri
Serial.println("writing");
sensorData = SD.open("json2.csv", FILE_WRITE);
 Serial.println(sensorData.available());
while (sensorData.available()) {
  Serial.println("data:");
      Serial.write(sensorData.read());
   }
}
else{
 
Serial.println(" No file");
}
}

//
void saveData(){
if(SD.exists("json2.csv")){ // check the card is still there
// now append new data filewri
Serial.println("writing");
sensorData = SD.open("json2.csv", FILE_WRITE);
if (sensorData){
 JSONencoder["emailId"] = "rarun.neer@gmail.com";
JSONencoder["password"] = "Tictoks@123";
char JSONmessageBuffer[300];
JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
//sensorData.println(String(JSONmessageBuffer));
String dat=String("rarun.neer@gmail.com") + "," +String("Tictoks@123");
sensorData.println(dat);

Serial.println(String(JSONmessageBuffer));
sensorData.close(); // close the file
}
}
else{
  sensorData = SD.open("json2.csv", FILE_WRITE);
if (sensorData){
JSONencoder["emailId"] = "rarun.neer@gmail.com";
JSONencoder["password"] = "Tictoks@123";
char JSONmessageBuffer[300];
JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
String dat=String("rarun.neer@gmail.com") + "," +String("Tictoks@123");
sensorData.println(dat);
sensorData.close(); // close the file
Serial.println(" created and writed!");
}
}
}
