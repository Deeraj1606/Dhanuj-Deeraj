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
    {"u3g7ry438ryu84irxxxxxxxx", "Relay 1", RELAYPIN_1},
    {"8u84fh84hxxxxxxxxxxxxxxx", "Relay 2", RELAYPIN_2}
};

#define WIFI_SSID         "SSID"
#define WIFI_PASS         "password"
#define APP_KEY           "APP KEY"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "SECRET KEY"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
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
