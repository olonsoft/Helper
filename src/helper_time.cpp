#include <helper_time.h>

namespace helper_time {


uint32_t getUpTimeSeconds() {
  static unsigned long last_uptime      = 0;
  static unsigned char uptime_overflows = 0;

  if (millis() < last_uptime) ++uptime_overflows;
  last_uptime = millis();
  unsigned long uptime_seconds =
      uptime_overflows * (UINT32_MAX / 1000) + (last_uptime / 1000);

  return uptime_seconds;
}

String getUpTimeString(){
  char buffer[15];  // "99999T23:59:59"

  uint32_t secs = getUpTimeSeconds();
  
  int _seconds  = secs % 60;
  secs         /= 60;          // now it is minutes
  int _minutes  = secs % 60;
  secs         /= 60;          // now it is hours
  int _hours    = secs % 24;
  int _days     = secs /= 24;  // now it is days
  
  /* 
  int _days           = secs / 86400;
  int _secsRemaining  = secs % 86400;
  int _hours          = _secsRemaining / 3600;
  _secsRemaining      = _secsRemaining % 3600;
  int _minutes        = _secsRemaining / 60;
  int _seconds        = _secsRemaining % 60;
  */
  snprintf_P(buffer, sizeof(buffer), PSTR("%dT%02d:%02d:%02d"), _days, _hours,
             _minutes, _seconds);
  return buffer;
}

String hourToString(time_t time1) {
  // Convert time_t to tm as UTC time
  struct tm *t = gmtime(&time1);

  char timeString[9];  // 8 digits plus the null char
  snprintf_P(timeString, sizeof(timeString), PSTR("%02d:%02d:%02d"), t->tm_hour,
             t->tm_min, t->tm_sec);
  return timeString;
}


String getLocalHourString() {
  time_t _time;
  // get number of seconds since 00:00 hours, 01/01/1970 UTC (unix timestamp)
  time(&_time); 
  return hourToString(_time);
}


String timeToString() {
  time_t t;
  time(&t);
  char buf[20];  // "2020-01-09T13:20:30" + 1;
  strftime(buf, sizeof(buf), "%FT%T", localtime(&t));
  return buf;
}

bool isLeapYear(int year) { return LEAP_YEAR(year); }


uint8_t getMonthDays(int year, uint8_t month) {
  const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  if (month == 1 && isLeapYear(year)) {
    return 29;
  }
  if (month > 11) {
    return 0;
  }
  return monthDays[month];
}

// ===============================

void setNextTimeInterval(uint32_t& timer, const uint32_t step) {
  timer += step;
  const long passed = timePassedSince(timer);
  if (passed < 0) {
    return;
  }

  if (static_cast<unsigned long>(passed) > step) {
    timer = millis() + step;
    return;
  }

  timer = millis() + (step - passed);
}


}