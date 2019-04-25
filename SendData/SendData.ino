 
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
/* Set these to your desired credentials. */
const char *ssid = "ARUN";  //ENTER YOUR WIFI SETTINGS
const char *password = "throughapps";

//Web/Server address to read/write from 
const char *host = "192.168.43.128";   //https://circuits4you.com website or IP address of server
StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();
int i=0;
//=======================================================================
//                    Power on setup
//=======================================================================
 
void setup() {
  delay(1000);
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
     //Declare object of class HTTPClient
 /*
  *  HTTPClient http;
  String ADCData, station, postData;
  int adcvalue=analogRead(A0);  //Read Analog value of LDR
  ADCData = String(adcvalue);   //String to interger conversion
  station = "A";
 Serial.print("status=" + ADCData + "&station=" + station);
  //Post Data
  postData = "status=" + ADCData + "&station=" + station ;
  
  http.begin("https://wildfly.tictoks.com:443/EmployeeAttendenceAPI/APICall/audrino?");              //Specify request destination
  http.addHeader("Content-Type", "application/json");    //Specify content-type header
  int httpCode = http.POST("loginId=audrinotestmail&password=audrino@123");   //Send the request
  http.writeToStream(&Serial);
  String payload = http.getString();    //Get the response payload
 
  //Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection

  */


  if(i==0){
   HTTPClient http;    //Declare object of class HTTPClient

JSONencoder["emailId"] = "rarun.neer@gmail.com";
JSONencoder["password"] = "Tictoks@123";
char JSONmessageBuffer[300];
JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
Serial.println(JSONmessageBuffer);

   http.begin("http://13.126.195.214:8080/EmployeeAttendenceAPI/employee/employeeLogin");      //Specify request destination
   http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
   int httpCode = http.POST(JSONmessageBuffer);   //Send the request
  /*  int httpCode = http.GET(JSONmessageBuffer);*/
   if(httpCode > 0){
    
 const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 600;
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
Serial.println("resturn"+http.getString());
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
  }
   http.end();  //Close connection
   Serial.println(httpCode); 
 i=1;
}
  delay(5000);  //Post Data at every 5 seconds
}
