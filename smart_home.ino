#ifdef ENABLE_DEBUG
  #define DEBUG_ESP_PORT Serial
  #define NODEBUG_WEBSOCKETS
  #define NDEBUG
#endif 

#include <Arduino.h>
#include <WiFi.h> // WiFi library for ESP32
#include <SinricPro.h>
#include <SinricProSwitch.h>

#define RELAYPIN_1 13 // Pin for Relay 1
#define RELAYPIN_2 12 // Pin for Relay 2

/*****************
 * Configuration *
 *****************/

struct RelayInfo {
  String deviceId;
  String name;
  int pin;
};

std::vector<RelayInfo> relays = {
    {"6613d5973019d22c41892b72", "Relay 1", RELAYPIN_1},
    {"6613e5e47c9e6c6fe866d93c", "Relay 2", RELAYPIN_2}
};

#define WIFI_SSID         "JET"
#define WIFI_PASS         "deeraj2010"
#define APP_KEY           "c4f9ded7-bcd9-4616-8574-9ccd5dad08a4"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "14654627-dd09-403c-a8b1-1858d86e8669-7f844194-737d-48b3-a121-c9c51a4f96aa"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define BAUD_RATE         115200              // Change baudrate to your need

bool onPowerState(const String &deviceId, bool &state) {
  for (auto &relay : relays) {
    if (deviceId == relay.deviceId) {
      Serial.printf("Device %s turned %s\r\n", relay.name.c_str(), state ? "on" : "off");
      digitalWrite(relay.pin, state ? HIGH : LOW); // Set relay state
      return true;
    }
  }
  return false;
}

void setupRelayPins() {
  for (auto &relay : relays) {
    pinMode(relay.pin, OUTPUT);
  }
}

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

void setupSinricPro() {
  for (auto &relay : relays) {
    SinricProSwitch &mySwitch = SinricPro[relay.deviceId];
    mySwitch.onPowerState(onPowerState);
  }

  SinricPro.onConnected([]() { Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([]() { Serial.printf("Disconnected from SinricPro\r\n"); });

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(BAUD_RATE);
  setupRelayPins();
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
