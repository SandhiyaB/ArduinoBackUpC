#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Keypad.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <SimpleTimer.h>
#include <ESP8266HTTPClient.h>
//Library for SD card
#include <SPI.h>
#include <SD.h>

//Starting Fingerprint
SoftwareSerial mySerial(13, 15);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//Object for file
File myFile;

//Object forJSON
StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();

// the timer object
SimpleTimer timer;

int id;
int idval = 0;

//Config Keypad Row and Column
const byte n_rows = 4;
const byte n_cols = 4;
int num[] = {0, 0, 0, 0};
char keys[n_rows][n_cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[n_rows] = {D3, D2, D1, D0};
byte rowPins[n_cols] = {D7, D6, D5, D4};

//For Getting Four Digit Number
int i = 0;
int firstvariable;

//Initializing Keypad
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);

//Object for Wifimanger
WiFiManager wifiManager;

//These Object Are Used to Check Internet is connected by opening Google.com

ESP8266WebServer server(80);
WiFiClient client;
char servername[] = "google.com";

//Setup Initialize all Device
void setup() {
  
  Serial.begin(9600);
  //Begining fingerprint
  finger.begin(57600);
 while (1) {
      delay(1);
    } 
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
   
  }
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  //reset settings - for testing
  wifiManager.resetSettings();
  WiFi.disconnect();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(180);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }
  if (!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  //Begining SD and checking Sd card is Initialized
  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

 //Timer which Call WifiStatusCheck to monitor wifi is connected
  timer.setInterval(60000, wifiStatusCheck);
}

void loop() {

  //For getting input from keypad
  char key1 = myKeypad.getKey();
  switch (key1) {

    case NO_KEY:
      break;

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      //lcd.print(key1);
      //After getting 4 digit num enroll fingerprint
      if (i == 3) {
        i = 0;
        firstvariable = firstvariable * 10 + key1 - '0';

        Serial.println("4 digit num ");
        Serial.print (firstvariable);
        Serial.print("Enrolling ID #");
        id = firstvariable;
        Serial.println(id);
        finger.getImage();
        while (!  getFingerprintEnroll() );

        firstvariable = 0;

      } else {
        firstvariable = firstvariable * 10 + key1 - '0';
        Serial.println("i value ");
        Serial.print (i);

        i = i + 1;
      }
      break;

      //For fingerprint matching
    case 'B':
      firstvariable = 0;
      Serial.println ();

      while (getFingerprintIDez()) {
        Serial.println("Found ID #");
        Serial.print(finger.fingerID);
        if (finger.fingerID != 0) {
          Serial.println("Found ID1 #");
          Serial.print(idval);
          Serial.print(" with confidence of "); Serial.println(finger.confidence);
          SendData(finger.fingerID);
          finger.fingerID = 0;
          break;
        }
      }
      break;
    case 'D ':
      firstvariable = 0;
      Serial.println ();

      while (getFingerprintIDez()) {
        Serial.println("Found ID #");
        Serial.print(finger.fingerID);
        if (finger.fingerID != 0) {
          Serial.println("Found ID1 #");
          Serial.print(idval);
          Serial.print(" with confidence of "); Serial.println(finger.confidence);
          SendData(finger.fingerID);
          finger.fingerID = 0;
          break;
        }
      }
      break;
    case 'C':
      firstvariable = 0;
      Serial.println ();

      while (getFingerprintIDez()) {
        Serial.println("Found ID #");
        Serial.print(finger.fingerID);
        if (finger.fingerID != 0) {
          Serial.println("Found ID1 #");
          Serial.print(idval);
          Serial.print(" with confidence of "); Serial.println(finger.confidence);
          SendData(finger.fingerID);
          finger.fingerID = 0;
          break;
        }
      }
      break;
//Reset 4 digit num to 0
    case '*':
      firstvariable = 0;
      // lcd.clear();
      Serial.println ();
      Serial.print ("Reset value:");
      Serial.print (firstvariable );
  }
  //  Running timer in loop
  timer.run();
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

//Function for Checking Wifi Status
void wifiStatusCheck() {
  Serial.println(WiFi.status());
  if (WiFi.status() == WL_CONNECTED) {
    if (client.connect(servername, 80)) {
      Serial.println("connected");
      // Make a HTTP request:
      client.println("GET /search?q=arduino HTTP/1.0");
      client.println();
    } else {
      Serial.println("not connected");

    }
    delay(1000);

    Serial.println("connecting...");

    if (client.connected()) {
      Serial.println("connected");
      client.println("GET /search?q=arduino HTTP/1.0");
      client.println();
    } else {
      Serial.println("connection failed");
    }
    Serial.println("Wifi Connected");
  } else {
    finger.begin(57600);

    if (finger.verifyPassword()) {
      Serial.println("Found fingerprint sensor!");
    } else {
      Serial.println("Did not find fingerprint sensor :(");
      while (1) {
        delay(1);
      }
    }
    // wifiManager.setTimeout(180);

    //fetches ssid and pass and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.resetSettings();
    WiFi.disconnect();
    ESP.reset();
    if (!wifiManager.autoConnect("AutoConnectAP")) {

      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
    if (!wifiManager.autoConnect("AutoConnectAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }



    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey  in loop:)");
  }

}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    Serial.print("p Status");
    Serial.print(String(p));
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  Serial.print("p Status 2nd ");
  Serial.println(String(p));
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  p = finger.loadModel(id);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("Template AFEER "); Serial.print(id); Serial.println(" loaded");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }
  Serial.print("Attempting to get #"); Serial.println(id);
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("Template "); Serial.print(id); Serial.println(" transferring:");
      break;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
  uint8_t bytesReceived[534]; // 2 data packets
  memset(bytesReceived, 0xff, 534);

  uint32_t starttime = millis();
  int i = 0;
  while (i < 534 && (millis() - starttime) < 20000) {
    if (mySerial.available()) {
      bytesReceived[i++] = mySerial.read();
    }
  }
  Serial.print(i); Serial.println(" bytes read.");
  Serial.println("Decoding packet...");

  uint8_t fingerTemplate[512]; // the real template
  memset(fingerTemplate, 0xff, 512);

  // filtering only the data packets
  int uindx = 9, index = 0;
  while (index < 534) {
    while (index < uindx) ++index;
    uindx += 256;
    while (index < uindx) {
      fingerTemplate[index++] = bytesReceived[index];
    }
    uindx += 2;
    while (index < uindx) ++index;
    uindx = index + 9;
  }
  for (int i = 0; i < 512; ++i) {
    //Serial.print("0x");
    printHex(fingerTemplate[i], 2);
    //Serial.print(", ");
  }
  Serial.println("\ndone.");

}
void printHex(int num, int precision) {
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
}

//After Verifing Fingerprint Send data to backend if internet connected else store in SD card
void SendData(int EmpId) {


  JSONencoder["emailId"] = "rarun.neer@gmail.com";
  JSONencoder["password"] = "Tictoks@123";
  char JSONmessageBuffer[300];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);
  if (WiFi.status() == WL_CONNECTED) {
    //Checking Network is connected
    if (client.connect(servername, 80)) {
      Serial.println("connected");
      // Make a HTTP request:
      client.println("GET /search?q=arduino HTTP/1.0");
      client.println();


      client.println();

      HTTPClient http;    //Declare object of class HTTPClient

      http.begin("http://13.126.195.214:8080/EmployeeAttendenceAPI/employee/employeeLogin");      //Specify request destination
      http.addHeader("Content-Type", "application/json");  //Specify content-type header

      int httpCode = http.POST(JSONmessageBuffer);   //Send the request
      const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
      DynamicJsonBuffer jsonBuffer(capacity);

      // Parse JSON object
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      const char*  code = root["employeeId"];
      const char*  department = root["department"];
      const char*  companyName = root["companyName"];
      const char*  companyType = root["companyType"];
      const char*  userName = root["userName"];
      const char*  employeeDepartmentlist = root["employeeDepartmentlist"];
      const char*  role = root["role"];
      Serial.println("resturn" + http.getString());
      Serial.print("Code return element = ");
      Serial.println("EmployeeId ");
      Serial.println(code);
      Serial.println("Employee name " );
      Serial.println(userName);
      Serial.println("companyName name " );
      Serial.println(companyName);
      Serial.println("companyType " );
      Serial.println(companyType);
      Serial.println("employeeDepartmentlist " );
      Serial.println(employeeDepartmentlist);
      Serial.println("Department" );
      Serial.println(department);
      Serial.println("Role" );
      Serial.println(role);
      Serial.println(httpCode);   //Print HTTP return code
      char filename[20];
      snprintf(filename, sizeof(filename), "data%03d.txt", EmpId);
      Serial.println("Filename");
      Serial.println(filename);
      myFile = SD.open(filename, FILE_WRITE);
      myFile.close();
      myFile = SD.open("test.txt", FILE_WRITE);
     // if the file opened okay, write to it:
      if (myFile) {
        Serial.print("Writing to test.txt...");
        myFile.println(JSONmessageBuffer);
        // close the file:
        myFile.close();
        myFile = SD.open(filename, FILE_WRITE);
        if (myFile) {
          Serial.println("test.txt");

          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
          Serial.println("done.");
        }

      } else {
        Serial.println("Failed to open.");
      }

      http.end();  //Close connection
    } else {
    //Network is not Connected So store file in backend
      Serial.println("connection failed");
      Serial.println(EmpId);
      if (!SD.begin(15)) {
        Serial.println("initialization failed!");
        return;
      }
      Serial.println("initialization done.");

      // String filename = "log"+String(EmpId) + ".txt";
      char filename[20];
      snprintf(filename, sizeof(filename), "data%03d.txt", EmpId);
      Serial.println("Filename");
      Serial.println(filename);
      myFile = SD.open(filename, FILE_WRITE);
      myFile.close();
      myFile = SD.open("test.txt", FILE_WRITE);

      // if the file opened okay, write to it:
      if (myFile) {
        Serial.print("Writing to test.txt...");
        myFile.println(JSONmessageBuffer);
        // close the file:
        myFile.close();
        myFile = SD.open(filename, FILE_WRITE);
        if (myFile) {
          Serial.println("test.txt:");

          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
          Serial.println("done.");
        }

      } else {
        Serial.println("Failed to open.");
      }
    }
  }
}
