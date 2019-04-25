#include <FPS_GT511C3.h>

/*
    This creates two empty databases, populates values, and retrieves them back
    from the SPIFFS file system.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <vfs.h>
#include <SPI.h>
#include <FS.h>
extern "C" {
#include "user_interface.h"
}
#include <ESP8266WiFi.h>

void WiFiOff() {
   wifi_station_disconnect();
   wifi_set_opmode(NULL_MODE);
   wifi_set_sleep_type(MODEM_SLEEP_T);
   wifi_fpm_open();
   wifi_fpm_do_sleep(0xFFFFFFF);
}

const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   Serial.printf("%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   return 0;
}

int db_open(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
       return rc;
   } else {
       Serial.printf("Opened database successfully\n");
   }
   return rc;
}

char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql) {
   Serial.println(sql);
   long start = micros();
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   } else {
       Serial.printf("Operation done successfully\n");
   }
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   return rc;
}

void setup() {

   Serial.begin(74880);
   sqlite3 *db1;
   sqlite3 *db2;
   int rc;

   system_update_cpu_freq(SYS_CPU_160MHZ);
   WiFiOff();

   if (!SPIFFS.begin()) {
       Serial.println("Failed to mount file system");
       return;
   }

   // list SPIFFS contents
   Dir dir = SPIFFS.openDir("/");
   while (dir.next()) {    
       String fileName = dir.fileName();
       size_t fileSize = dir.fileSize();
       Serial.printf("FS File: %s, size: %ld\n", fileName.c_str(), (long) fileSize);
   }
   Serial.printf("\n");

   // remove existing file
 //  SPIFFS.remove("/test1.db");
  // SPIFFS.remove("/DataTable.db");
//SPIFFS.remove("/EmployeeTable.db");
  // SPIFFS.remove("/Offline.db");

   sqlite3_initialize();

   // Open databases
   File db_file_obj_1;
   vfs_set_spiffs_file_obj(&db_file_obj_1);
   if (db_open("/FLASH/EmployeeTable.db", &db1))
       return;
   File db_file_obj_2;
   vfs_set_spiffs_file_obj(&db_file_obj_2);
   if (db_open("/FLASH/Offline.db", &db2))
       return; 
   rc = db_exec(db2, "CREATE TABLE if not exists EmployeeTable(Id INTEGER PRIMARY KEY, Data text,CheckInOut text);");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   

   rc = db_exec(db1, "CREATE TABLE if not exists Offline(Id INTEGER PRIMARY KEY, EmployeeId integer,FingerPrint1 BLOB,RFIDNo text);");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
  

   rc = db_exec(db1, "INSERT INTO EmployeeTable (EmployeeId,RFIDNo) VALUES ('1', '123456');");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   rc = db_exec(db2, "INSERT INTO Offline (Data,CheckInOut) VALUES ('{id:1,name=sandy}', 'RFID');");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }

   rc = db_exec(db1, "SELECT * FROM sqlite_master WHERE type='table' AND name='test1';");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   
   rc = db_exec(db1, "SELECT * FROM EmployeeTable");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   rc = db_exec(db2, "SELECT * FROM Offline");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }

   sqlite3_close(db1);
   sqlite3_close(db2);

}

void loop() {
}
