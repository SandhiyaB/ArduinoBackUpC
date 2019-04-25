#include "Arduino.h"
#include <EDB.h>
#include <EEPROM.h>
#include <SD.h>
#include <string.h>

File dbFile;

#define TABLE_SIZE 512
#define RECORDS_TO_CREATE 10


struct LogEvent {
 int id;
 int temperature;
}
logEvent;

struct Transponder {
 char date[11];
 char time[9]; 
  int tf1;
} transponder;

int recno;

void writer(unsigned long address, byte data)
{
 dbFile.seek(address);
 dbFile.write(data);
 dbFile.flush();
}

byte reader(unsigned long address)
{
 dbFile.seek(address);
 return dbFile.read();
}

EDB db(&writer, &reader);

void setup()
{
 Serial.begin(9600);
 
 Serial.print("Initializing SD card...");
 pinMode(10, OUTPUT);
 
 if (SD.begin(15)) {
   Serial.println("initialization failed!");
   return;
 }
 
 Serial.println("initialization done.");
 //SD.remove("transp.db"); //just for testing to delete db and recreate
 Serial.println("Opening transp.db ...");
 if (SD.exists("transp.db")) {
   Serial.println("transp.db exists. Open it...");
   dbFile = SD.open("transp.db");
   db.open(0);
 }
 else {
   Serial.println("transp.db doesn't exist. Create it...");
   dbFile = SD.open("transp.db", FILE_WRITE);
   db.create(0, TABLE_SIZE, sizeof(transponder));
   Serial.println("Creating Records...");
   for (recno = 1; recno <= RECORDS_TO_CREATE; recno++)
   {
     int m = random(1, 12);
   int d = random(1, 31);
   int h = random(1, 12);
   int i = random(59);
   int s = random(59);
   sprintf(transponder.date, "2009-%02d-%02d", m, d);
   sprintf(transponder.time, "%02d:%02d:%02d", h, i, s);

     transponder.tf1 = 1;
     db.appendRec(EDB_REC transponder);
   }
 }

 Serial.print("Record Count: "); Serial.println(db.count());
 for (recno = 1; recno < db.count(); recno++)
 {
   db.readRec(recno, EDB_REC transponder);
   Serial.print("ID: "); Serial.println(recno);
   Serial.print("Date: "); Serial.println(transponder.date); 
   Serial.print("Time: "); Serial.println(transponder.time);
   Serial.print("Timeframe 1: "); Serial.println(transponder.tf1); 
 }


 dbFile.close();
 delay(2000); //Test database with reopen...
 Serial.println("Opening transp.db ...");
 if (SD.exists("transp.db")) {
   Serial.println("transp.db exists. Open it...");
   dbFile = SD.open("transp.db");
   db.open(0);
 }
 Serial.print("Record Count: "); Serial.println(db.count());
 for (recno = 1; recno < db.count(); recno++)
 {
   db.readRec(recno, EDB_REC transponder);
   Serial.print("ID: "); Serial.println(recno);
   Serial.print("Date: "); Serial.println(transponder.date); 
   Serial.print("Time: "); Serial.println(transponder.time);
   Serial.print("Timeframe 1: "); Serial.println(transponder.tf1); 
 }
}

void loop()
{

}
