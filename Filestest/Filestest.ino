/*
  SD card basic file example

  This example shows how to create and destroy an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/
#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("test.txt")) {
    Serial.println("test.txt exists.");
  } else {
    Serial.println("test.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial.println("Creating example.txt...");
  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists:
  if (SD.exists("test.txt")) {
    Serial.println("test.txt exists.");
  } else {
    Serial.println("test.txt doesn't exist.");
  }
   myFile = SD.open("test.txt");
if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  // delete the file:
  Serial.println("Removing test.txt...");
  SD.remove("test.txt");

  if (SD.exists("test.txt")) {
    Serial.println("test.txt exists.");
  } else {
    Serial.println("test.txt doesn't exist.");
  }
   myFile = SD.open("transp.db");
   
  if (myFile) {
    Serial.println("transp.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
        Serial.print("Record Count: "); Serial.println(db.count());
 for (recno = 1; recno < db.count(); recno++)
 {
   db.readRec(recno, EDB_REC transponder);
   Serial.print("ID: "); Serial.println(recno);
   Serial.print("Date: "); Serial.println(transponder.date); 
   Serial.print("Time: "); Serial.println(transponder.time);
   Serial.print("Timeframe 1: "); Serial.println(transponder.tf1); 
 }

      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening transp.txt");
  }
}

void loop() {
  // nothing happens after setup finishes.
}
