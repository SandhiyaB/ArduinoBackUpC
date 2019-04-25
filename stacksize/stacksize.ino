#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

extern "C" {
#include <cont.h>
  extern cont_t g_cont;
}

// CASE 0: no reinitialization
// CASE 1: reinitialize from an implicit temporary instance created on the stack
// CASE 2: reinitialize from an explicit temporary instance created on the heap
// CASE 3: reinitialize with "placement new" (no temporary instance)
#define CASE 1

#define WIFI_SSID "..."
#define WIFI_PASS "..."

ESP8266WebServer server;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("OK");

#if CASE == 0
  /* no code */
#elif CASE == 1
  server = ESP8266WebServer(WiFi.softAPIP());
#elif CASE == 2
  ESP8266WebServer *tmp = new ESP8266WebServer(WiFi.softAPIP());
  server = *tmp;
  delete tmp;
#elif CASE == 3
  server.~ESP8266WebServer();
  new (&server) ESP8266WebServer(WiFi.softAPIP());
#endif

  server.begin();

  Serial.printf("unmodified stack   = %4d\n", cont_get_free_stack(&g_cont));
  register uint32_t *sp asm("a1");
  Serial.printf("current free stack = %4d\n", 4 * (sp - g_cont.stack));
}

void loop() {
  // do nothing
  delay(1000);
}
