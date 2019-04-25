#include <SPI.h>
#include <SD.h>

#include <SoftwareSerial.h>
#include <Wire.h>//import necessary libraries: SoftwareSerial, Wire
//#include <DS3231.h>
#include "Arduino.h" 
#include <EDB.h> // for database and SD card
#define SD_PIN 15  // SD Card CS pin
//#define TABLE_SIZE 8192 // for database and SD card
#define TABLE_SIZE 10000 // for database and SD card

// Init the DS3231 using the hardware interface
//DS3231  rtc(SDA, SCL);


const int soilMoist = 0;
// const long int recordPeriod = 43200000; // 12 hours
const long int recordPeriod = 1000;
char val; // variable to receive data from the serial port
char* db_name = "/db/edb_test.db"; // path of db files in SD card
File dbFile;  
int logid; // variable to store id of log

// Arbitrary record definition for this table.
// This should be modified to reflect your record needs.
struct LogEvent {
    int id;
    String datas;
    
}
logEvent;

// The read and write handlers for using the SD Library
// Also blinks the led while writing/reading
void writer (unsigned long address, byte data) {
    dbFile.seek(address);
    dbFile.write(data);
    dbFile.flush();
}

byte reader (unsigned long address) {
    dbFile.seek(address);
    byte b = dbFile.read();
    return b;
}

// Create an EDB object with the appropriate write and read handlers
EDB db(&writer, &reader);

void setup() {
  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  // Initialize the rtc object
//  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(SUNDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(9, 1, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(31, 7, 2016);   // Set the date to January 1st, 2014
  
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  if (!SD.begin(SD_PIN)) {
      Serial.println("No SD-card.");
      return;
  }

  // Check dir for db files
  if (!SD.exists("/db")) {
      Serial.println("Dir for Db files does not exist, creating...");
      SD.mkdir("/db");
  }
  if (SD.exists(db_name)) {
      dbFile = SD.open(db_name, FILE_WRITE);
      // Sometimes it wont open at first attempt, espessialy after cold start
      // Let's try one more time
      if (!dbFile) {
          dbFile = SD.open(db_name, FILE_WRITE);
      }

      if (dbFile) {
          Serial.print(F("Openning current table... "));
          EDB_Status result = db.open(0);
          if (result == EDB_OK) {
              Serial.println(F("DONE"));
          } else {
              Serial.println(F("ERROR"));
              Serial.println("Did not find database in the file " + String(db_name));
              Serial.print(F("Creating new table... "));
              db.create(0, TABLE_SIZE, (unsigned int)sizeof(logEvent));
              Serial.println(F("DONE"));
              return;
          }
      } else {
            Serial.println("Could not open file " + String(db_name));
            return;
      }
      } else {
        Serial.print("Creating table... ");
        // create table at with starting address 0
        dbFile = SD.open(db_name, FILE_WRITE);
        db.create(0, TABLE_SIZE, (unsigned int)sizeof(logEvent));
        Serial.println("DONE");
     }    
  recordLimit();    
  deleteAll();
  
  // read sensors and append to the string:
  int sensor;
  String ltime;
  sensor = analogRead(soilMoist);
  logEvent.id = logid;
  logid = db.count()+1;
  //logEvent.logdate  = rtc.getDateStr();
//  logEvent.logtime = rtc.getTimeStr();
  Serial.println("TIME"+logEvent.datas);
 

  EDB_Status result = db.appendRec(EDB_REC logEvent);
  if (result != EDB_OK) printError(result);
  Serial.println(F("Append 1 record"));    
  delay(5000); 
  // read sensors and append to the string:
  sensor = analogRead(soilMoist);
  logEvent.id = logid;
  logid = db.count()+1;
  //logEvent.logdate  = rtc.getDateStr();
//  logEvent.logtime = rtc.getTimeStr();
  Serial.println("TIME"+logEvent.datas);
  //logEvent.temp =  int(ktc.readCelsius()); // temperature sensor data
  result = db.appendRec(EDB_REC logEvent);
  if (result != EDB_OK) printError(result);
  Serial.println(F("Append 1 record"));    
  delay(5000); 
  selectAll();
  dbFile.close();
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";
 
  byte cmmd[20]; 
  int insize; 
    
 
    dataString += String(insize);
    dataString += ",";
     for (int i=0; i<insize; i++) {
// cmmd[i]=char(I2CBT.read());//將接收的訊息顯示出來
       dataString += cmmd[i];
       dataString += ",";
     }
  
  delay(1000); 
   
}

void recordLimit()
{
    Serial.print(F("Record Limit: "));
    Serial.println(db.limit());
}

void countRecords()
{
    Serial.print(F("Record Count: "));
    Serial.println(db.count());
}

void printError(EDB_Status err)
{
    Serial.print(F("ERROR: "));
    switch (err)
    {
        case EDB_OUT_OF_RANGE:
            Serial.println(F("Recno out of range"));
            break;
        case EDB_TABLE_FULL:
            Serial.println(F("Table full"));
            break;
        case EDB_OK:
        default:
            Serial.println(F("OK"));
            break;
    }
}

void selectAll()
{
    for (int recno = 1; recno <= db.count(); recno++)
    {
  EDB_Status result = db.readRec(recno, EDB_REC logEvent);
        if (result == EDB_OK)
        {
            Serial.print(F("Recno: "));
            Serial.print(recno);
            Serial.print(F(" ID: "));
            Serial.print(logEvent.id);
            //Serial.print(F(" Date: "));
            //Serial.print(logEvent.logdate);
            Serial.print(F(" Time: "));
            Serial.print(logEvent.datas);
            
        }
        else printError(result);
    }
}

void deleteAll()
{
    Serial.print("Truncating table... ");
    db.clear();
    Serial.println("DONE");
}
