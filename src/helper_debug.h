#pragma once

#include <time_functions.h>

#ifdef APP_DEBUG_PORT
  #define DBG_PORT      APP_DEBUG_PORT
#else
  #define DBG_PORT      Serial
#endif

// 0: DISABLED: no logging
// 1: ERROR: (red) Critical errors, software module can not recover on its own
// 2: WARN:  (yellow) Error conditions from which recovery measures have been taken
// 3: INFO:  (green) Information messages which describe normal flow of events
// 4: DEBUG: (blue) Extra information which is not necessary for normal use (values, pointers, sizes, etc)
// 5: VERBOSE: (cyan) Bigger chunks of debugging information, or frequent messages which can potentially flood the output.

#ifndef APP_LOG_LEVEL
  #define APP_LOG_LEVEL      4
#endif

#define TERMINAL_IN_COLOR
// to print color ansi chars on platformio terminal 
// set monitor_flags  = --raw in platformio.ini
// https://en.wikipedia.org/wiki/ANSI_escape_code

#define TERMINAL_COLOR_RESET    "\e[m"   // reset default
#define TERMINAL_COLOR_CYAN     "\e[36m" // 96 or 36 (bright vs normal)
#define TERMINAL_COLOR_BLUE     "\e[34m" // 94 or 34
#define TERMINAL_COLOR_GREEN    "\e[92m" // 92 or 32
#define TERMINAL_COLOR_YELLOW   "\e[93m" // 93 or 33
#define TERMINAL_COLOR_RED      "\e[31m" // 91 or 31
#define TERMINAL_COLOR_WHITE    "\e[37m" // 97 or 37

#define TERMINAL_COLOR_TIME     "\e[36m" // 36 light cyan
#define TERMINAL_COLOR_FUNCTION "\e[35m" // magenta

#ifdef TERMINAL_IN_COLOR
#define TERMINAL_COLOR_ERROR    "[" TERMINAL_COLOR_RED "E" TERMINAL_COLOR_RESET "] "
#define TERMINAL_COLOR_WARN     "[" TERMINAL_COLOR_YELLOW "W" TERMINAL_COLOR_RESET "] "
#define TERMINAL_COLOR_INFO     "[" TERMINAL_COLOR_GREEN "I" TERMINAL_COLOR_RESET "] "
#define TERMINAL_COLOR_DEBUG    "[" TERMINAL_COLOR_BLUE "D" TERMINAL_COLOR_RESET "] "
#define TERMINAL_COLOR_VERBOSE  "[" TERMINAL_COLOR_CYAN "V" TERMINAL_COLOR_RESET "] "

#ifdef TERMINAL_TIME_PRINT
#define TIMESTAMP_PRINT      DBG_PORT.printf("[" TERMINAL_COLOR_TIME "%s" TERMINAL_COLOR_RESET "] ", timeToString().c_str())
#else
#define TIMESTAMP_PRINT      DBG_PORT.printf(("[" TERMINAL_COLOR_TIME "%10lu" TERMINAL_COLOR_RESET "] ", millis())
#endif

#else

#define TERMINAL_COLOR_ERROR "[E] "
#define TERMINAL_COLOR_WARN  "[W] "
#define TERMINAL_COLOR_INFO  "[I] "
#define TERMINAL_COLOR_DEBUG "[D] "
#define TERMINAL_COLOR_DEBUG "[V] "
#ifdef TERMINAL_TIME_PRINT
#define TIMESTAMP_PRINT      DBG_PORT.printf("[%s] ", timeToString().c_str())
#else
#define TIMESTAMP_PRINT      DBG_PORT.printf("[%10lu] ", millis())
#endif
#endif

// todo: use __PRETTY_FUNCTION__ , __FILE__ , __LINE__ , __func__

// log errors
#define LOGERROR(x)       if (APP_LOG_LEVEL>0) { DBG_PORT.print(x); }
#define LOGERRORLN(x)     if (APP_LOG_LEVEL>0) { DBG_PORT.println(x); }
#define LOGERRORF(...)    if (APP_LOG_LEVEL>0) { DBG_PORT.printf(__VA_ARGS__); }
#define LOGERRORF_P(...)  if (APP_LOG_LEVEL>0) { DBG_PORT.printf_P(__VA_ARGS__); }
// log errors with time stamp
#define TLOGERROR(x)      if (APP_LOG_LEVEL>0) { TIMESTAMP_PRINT; LOGERROR(x); }
#define TLOGERRORLN(x)    if (APP_LOG_LEVEL>0) { TIMESTAMP_PRINT; LOGERRORLN(x);  }
#define TLOGERRORF(...)   if (APP_LOG_LEVEL>0) { TIMESTAMP_PRINT; LOGERRORF(...); }
#define TLOGERRORF_P(...) if (APP_LOG_LEVEL>0) { TIMESTAMP_PRINT; LOGERRORF_P(...); }

// log warnings
#define LOGWARN(x)        if (APP_LOG_LEVEL>1) { DBG_PORT.print(x); }
#define LOGWARNLN(x)      if (APP_LOG_LEVEL>1) { DBG_PORT.println(x); }
#define LOGWARNF(...)     if (APP_LOG_LEVEL>1) { DBG_PORT.printf(__VA_ARGS__); }
#define LOGWARNF_P(...)   if (APP_LOG_LEVEL>1) { DBG_PORT.printf_P(__VA_ARGS__); }
// log wanrings with timestamp
#define TLOGWARN(x)       if (APP_LOG_LEVEL>1) { TIMESTAMP_PRINT; LOGWARN(x); }
#define TLOGWARNLN(x)     if (APP_LOG_LEVEL>1) { TIMESTAMP_PRINT; LOGWARNLN(x);  }
#define TLOGWARNF(...)    if (APP_LOG_LEVEL>1) { TIMESTAMP_PRINT; LOGWARNF(...); }
#define TLOGWARNF_P(...)  if (APP_LOG_LEVEL>1) { TIMESTAMP_PRINT; LOGWARNF_P(...); }

//log info
#define LOGINFO(x)        if (APP_LOG_LEVEL>2) { DBG_PORT.print(x); }
#define LOGINFOLN(x)      if (APP_LOG_LEVEL>2) { DBG_PORT.println(x); }
#define LOGINFOF(...)     if (APP_LOG_LEVEL>2) { DBG_PORT.printf(__VA_ARGS__); }
#define LOGINFOF_P(...)   if (APP_LOG_LEVEL>2) { DBG_PORT.printf_P(__VA_ARGS__); }
// log info with timestamp
#define TLOGINFO(x)       if (APP_LOG_LEVEL>2) { TIMESTAMP_PRINT; LOGINFO(x); }
#define TLOGINFOLN(x)     if (APP_LOG_LEVEL>2) { TIMESTAMP_PRINT; LOGINFOLN(x);  }
#define TLOGINFOF(...)    if (APP_LOG_LEVEL>2) { TIMESTAMP_PRINT; LOGINFOF(...); }
#define TLOGINFOF_P(...)  if (APP_LOG_LEVEL>2) { TIMESTAMP_PRINT; LOGINFOF_P(...); }

//log debug
#define LOGDEBUG(x)       if (APP_LOG_LEVEL>3) { DBG_PORT.print(x); }
#define LOGDEBUGLN(x)     if (APP_LOG_LEVEL>3) { DBG_PORT.println(x); }
#define LOGDEBUGF(...)    if (APP_LOG_LEVEL>3) { DBG_PORT.printf(__VA_ARGS__); }
#define LOGDEBUGF_P(...)  if (APP_LOG_LEVEL>3) { DBG_PORT.printf_P(__VA_ARGS__); }
//log debug with timestamp
#define TLOGDEBUG(x)      if (APP_LOG_LEVEL>3) { TIMESTAMP_PRINT; LOGDEBUG(x); }
#define TLOGDEBUGLN(x)    if (APP_LOG_LEVEL>3) { TIMESTAMP_PRINT; LOGDEBUGLN(x);  }
#define TLOGDEBUGF(...)   if (APP_LOG_LEVEL>3) { TIMESTAMP_PRINT; LOGDEBUGF(...); }
#define TLOGDEBUGF_P(...) if (APP_LOG_LEVEL>3) { TIMESTAMP_PRINT; LOGDEBUGF_P(...); }
