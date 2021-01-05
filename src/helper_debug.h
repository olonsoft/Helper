#pragma once

#include <time_functions.h>

#ifdef APP_DEBUG_PORT
  #define DBG_PORT      APP_DEBUG_PORT
#else
  #define DBG_PORT      Serial
#endif

// todo: use __PRETTY_FUNCTION__ , __FILE__ , __LINE__ , __func__

// to print color ansi chars on platformio terminal 
// set monitor_flags  = --raw in platformio.ini
// https://en.wikipedia.org/wiki/ANSI_escape_code
#define TIME_PRINT
#ifdef TIME_PRINT
#define TIMESTAMP_PRINT      DBG_PORT.printf("[\e[36m%s\e[m] ", timeToString().c_str())
#else
#define TIMESTAMP_PRINT      DBG_PORT.printf("[\e[36m%10lu\e[m] ", millis())
#endif

// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN:  errors and warnings
// 3: INFO:  errors, warnings and informational
// 4: DEBUG: errors, warnings, informational and debug

#ifndef APP_LOG_LEVEL
  #define APP_LOG_LEVEL      4
#endif

// log errors
#define LOGERROR(x)       if (APP_LOG_LEVEL>0) { DBG_PORT.print(x); }
#define LOGERRORLN(x)     if (APP_LOG_LEVEL>0) { DBG_PORT.println(x); }
#define LOGERRORF(...)    if (APP_LOG_LEVEL>0) { DBG_PORT.printf(__VA_ARGS__); }
#define LOGERRORF_P(...)  if (APP_LOG_LEVEL>0) { DBG_PORT.printf_P(__VA_ARGS__); }
// log errors with time stamp
#define TLOGERROR(x)      if (APP_LOG_LEVEL>0) { TIMESTAMP_PRINT; DBG_PORT.print(x); }
#define TLOGERRORLN(x)    if (APP_LOG_LEVEL>0) { TIMESTAMP_PRINT; DBG_PORT.println(x);  }
#define TLOGERRORF(...)   if (APP_LOG_LEVEL>0) { TIMESTAMP_PRINT; DBG_PORT.printf(__VA_ARGS__); }
#define TLOGERRORF_P(...) if (APP_LOG_LEVEL>0) { TIMESTAMP_PRINT; DBG_PORT.printf_P(__VA_ARGS__); }

// log warnings
#define LOGWARN(x)        if (APP_LOG_LEVEL>1) { DBG_PORT.print(x); }
#define LOGWARNLN(x)      if (APP_LOG_LEVEL>1) { DBG_PORT.println(x); }
#define LOGWARNF(...)     if (APP_LOG_LEVEL>1) { DBG_PORT.printf(__VA_ARGS__); }
#define LOGWARNF_P(...)   if (APP_LOG_LEVEL>1) { DBG_PORT.printf_P(__VA_ARGS__); }
// log wanrings with timestamp
#define TLOGWARN(x)       if (APP_LOG_LEVEL>1) { TIMESTAMP_PRINT; DBG_PORT.print(x); }
#define TLOGWARNLN(x)     if (APP_LOG_LEVEL>1) { TIMESTAMP_PRINT; DBG_PORT.println(x);  }
#define TLOGWARNF(...)    if (APP_LOG_LEVEL>1) { TIMESTAMP_PRINT; DBG_PORT.printf(__VA_ARGS__); }
#define TLOGWARNF_P(...)  if (APP_LOG_LEVEL>1) { TIMESTAMP_PRINT; DBG_PORT.printf_P(__VA_ARGS__); }

//log info
#define LOGINFO(x)        if (APP_LOG_LEVEL>2) { DBG_PORT.print(x); }
#define LOGINFOLN(x)      if (APP_LOG_LEVEL>2) { DBG_PORT.println(x); }
#define LOGINFOF(...)     if (APP_LOG_LEVEL>2) { DBG_PORT.printf(__VA_ARGS__); }
#define LOGINFOF_P(...)   if (APP_LOG_LEVEL>2) { DBG_PORT.printf_P(__VA_ARGS__); }
// log info with timestamp
#define TLOGINFO(x)       if (APP_LOG_LEVEL>2) { TIMESTAMP_PRINT; DBG_PORT.print(x); }
#define TLOGINFOLN(x)     if (APP_LOG_LEVEL>2) { TIMESTAMP_PRINT; DBG_PORT.println(x);  }
#define TLOGINFOF(...)    if (APP_LOG_LEVEL>2) { TIMESTAMP_PRINT; DBG_PORT.printf(__VA_ARGS__); }
#define TLOGINFOF_P(...)  if (APP_LOG_LEVEL>2) { TIMESTAMP_PRINT; DBG_PORT.printf_P(__VA_ARGS__); }

//log debug
#define LOGDEBUG(x)       if (APP_LOG_LEVEL>3) { DBG_PORT.print(x); }
#define LOGDEBUGLN(x)     if (APP_LOG_LEVEL>3) { DBG_PORT.println(x); }
#define LOGDEBUGF(...)    if (APP_LOG_LEVEL>3) { DBG_PORT.printf(__VA_ARGS__); }
#define LOGDEBUGF_P(...)  if (APP_LOG_LEVEL>3) { DBG_PORT.printf_P(__VA_ARGS__); }
//log debug with timestamp
#define TLOGDEBUG(x)      if (APP_LOG_LEVEL>3) { TIMESTAMP_PRINT; DBG_PORT.print(x); }
#define TLOGDEBUGLN(x)    if (APP_LOG_LEVEL>3) { TIMESTAMP_PRINT; DBG_PORT.println(x);  }
#define TLOGDEBUGF(...)   if (APP_LOG_LEVEL>3) { TIMESTAMP_PRINT; DBG_PORT.printf(__VA_ARGS__); }
#define TLOGDEBUGF_P(...) if (APP_LOG_LEVEL>3) { TIMESTAMP_PRINT; DBG_PORT.printf_P(__VA_ARGS__); }
