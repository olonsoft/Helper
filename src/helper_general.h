#pragma once

#include <helper_debug.h>

#include <FS.h>

#ifdef ESP32
#include <WiFi.h>
#include <LITTLEFS.h>
#define ESP_FS LITTLEFS
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#define ESP_FS LittleFS
#endif

#define FORMAT_LITTLEFS_IF_FAILED true
#define DEVICE_ID_STR             "{id}"
#define SUCCESS_STR               "Success"
#define FAIL_STR                  "Fail"

#define CSTR(x) x.c_str()

#define BIT_SET(value, pos)    (value |= (1ULL << pos))
#define BIT_CLEAR(value, pos)  (value &= (~(1ULL << pos)))
#define BIT_TOGGLE(value, pos) (value ^= (1ULL << pos))
#define BIT_IS_SET(value, pos) (!!(value) & (1ULL << pos))

namespace helper_general {

uint32_t getChipIdInt();
String   getChipIdHex();
String   getChipIdStr();
String   getDeviceName(String name);
String   getDefaultDeviceName();
String   getSystemInfoJson();
String   getSystemStatusJson(int errWiFi, int errMqtt);
String   addTrailingSlash(String str);
bool     isdigit(char n);
String   boolToString(bool b, String t, String f);
String   boolToString(bool b);
String   boolSuccess(bool b);
String   intToString(int32_t i);
String   quotedText(String s);
String   quotedText(const char* s);
String   jsonPair(String s1, String s2);
String   jsonPair(String s1, int i);
String   jsonPair(String s1, bool b);
String   jsonPair(String s1, float f);
String   jsonPair(const char* s1, const char* s2);
bool     isTrue(String s);
String   TwoDigitNumber(int n);
String   formatBytes(size_t bytes);
bool     beginFS();

extern String lastErrorStr;
extern bool fileSystemBegun;

}