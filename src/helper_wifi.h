#pragma once 

#ifdef ESP32
//#include <ESPmDNS.h>
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
#endif

namespace helper_wifi {

bool wifiIsAP(WiFiMode_t wifimode);
bool wifiIsSTA(WiFiMode_t wifimode);
bool wifiValidIP();
bool wifiConnected();
int  wifiGetRssiAsQuality(int rssi);
String getWifiModeString(WiFiMode_t wifimode);
String getEncryptionTypeStr(uint8_t authmode);

}