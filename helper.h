#pragma once

#ifdef ESP32
#include <SPIFFS.h>
#define ESP_FS SPIFFS
#elif defined(ESP8266)
//#include <FS.h>
#include <LittleFS.h>
#define ESP_FS LittleFS
#endif

#define MILLIS_PRINT         Serial.printf("[%10lu] ", millis())

#define ENABLE_DEBUG_PRINT

#ifdef ENABLE_DEBUG_PRINT
#define TDEBUG_PRINT(x)      MILLIS_PRINT; Serial.print(x)
#define TDEBUG_PRINTLN(x)    MILLIS_PRINT; Serial.println(x)
#define TDEBUG_PRINTF(...)   MILLIS_PRINT; Serial.printf(__VA_ARGS__)
#define TDEBUG_PRINTF_P(...) MILLIS_PRINT; Serial.printf_P(__VA_ARGS__) 
#define DEBUG_PRINT(x)       Serial.print(x)
#define DEBUG_PRINTLN(x)     Serial.println(x)
#define DEBUG_PRINTF(...)    Serial.printf(__VA_ARGS__)
#define DEBUG_PRINTF_P(...)  Serial.printf_P(__VA_ARGS__) 
#else
#define TDEBUG_PRINT(x)
#define TDEBUG_PRINTLN(x) 
#define TDEBUG_PRINTF(...)
#define TDEBUG_PRINTF_P(...)
#define DEBUG_PRINT(x) 
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(...)
#define DEBUG_PRINTF_P(...)
#endif

// todo: use __PRETTY_FUNCTION__ , __FILE__ , __LINE__ , __func__
#define CSTR(x) x.c_str()

#define BIT_SET(value, pos)    (value |= (1ULL << pos))
#define BIT_CLEAR(value, pos)  (value &= (~(1ULL << pos)))
#define BIT_TOGGLE(value, pos) (value ^= (1ULL << pos))
#define BIT_IS_SET(value, pos) (!!(value) & (1ULL << pos))

#define SUCCESS_STR "Success"
#define FAIL_STR    "Fail"

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

extern String lastErrorStr;
