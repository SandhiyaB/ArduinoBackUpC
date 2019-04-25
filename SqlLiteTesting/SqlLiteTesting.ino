#include <SD.h>

#include <sqlite3.h>
#include <vfs.h>
#include <SPI.h>
#define FS_NO_GLOBALS

#include <FS.h>
extern "C" {
#include "user_interface.h"
}


const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName) {
  int i;
  Serial.printf("%s: ", (const char*)data);
  for (i = 0; i < argc; i++) {
    Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  Serial.printf("\n");
  return 0;
}

int openDb(char *filename, sqlite3 **db) {
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
  Serial.println(micros() - start);
  return rc;
}
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

  File myFile;

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  if (SD.exists("/SD0")) {
    Serial.println("Sql.db exists.");
  } else {
    SD.mkdir("/SD0");
    
    Serial.println("Sql.db doesn't exist.");
  }
  myFile = SD.open("/SD0/Sql.db", FILE_WRITE);
  myFile.close();

  if (SD.exists("/SD0/Sql.db")) {
    Serial.println("Sql.db exists.");
  } else {
    Serial.println("Sql.db doesn't exist.");
  }
  Serial.println("heap size");
Serial.println(ESP.getFreeHeap());
  sqlite3 *db1;
  int rc;
  Serial.println("bef spiffs");
  system_update_cpu_freq(SYS_CPU_160MHZ);
  SPI.begin();
  vfs_mount("D;/", SS);

  Serial.println("aft spiffs");
  sqlite3_initialize();
  Serial.println("aft initial");
   // rc = sqlite3_open("Sql.db", &db1);
  rc = sqlite3_open_v2("/SD0/Sql.db", &db1, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI, NULL);
  if (rc) {
       Serial.printf("Can't open database: %s\n");
       
   } else {
       Serial.printf("Opened database successfully\n");
   }
  Serial.println("aft db");
  Serial.println(rc);
  // Open databases
 
  if (openDb("/SD0/TicToks.db", &db1))
    return;
 Serial.println("aft db");
  Serial.println(rc);
 
/*

  rc = db_exec(db1, "CREATE TABLE EmployeeAttendanceSqlTable (id INTEGER, content);");
  if (rc != SQLITE_OK) {
    sqlite3_close(db1);
    return;
  }
  rc = db_exec(db1, "INSERT INTO EmployeeAttendanceSqlTable VALUES (1, 'Json Data');");
  if (rc != SQLITE_OK) {
    sqlite3_close(db1);
    return;
  }
  rc = db_exec(db1, "SELECT * FROM EmployeeAttendanceSqlTable");
  if (rc != SQLITE_OK) {
    sqlite3_close(db1);
    return;
  }
  sqlite3_close(db1);
  */
}
void loop() {
}
