#pragma once

#include <Arduino.h>
#include <time.h>

#define SECS_PER_MIN ((uint32_t)(60UL))
#define SECS_PER_HOUR ((uint32_t)(3600UL))
#define SECS_PER_DAY ((uint32_t)(SECS_PER_HOUR * 24UL))
#define MINS_PER_HOUR ((uint32_t)(60UL))
#define LEAP_YEAR(Y)                        \
  (((1970 + Y) > 0) && !((1970 + Y) % 4) && \
   (((1970 + Y) % 100) || !((1970 + Y) % 400)))

// max 49710 days
// UINT32_MAX = 0xFFFFFFFFUL = 4294967295

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

String   getUpTimeString(){
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


long IRAM_ATTR timeDiff(const unsigned long prev,
                              const unsigned long next) {
  long signed_diff = 0;

  // To cast a value to a signed long, the difference may not exceed half the
  // ULONG_MAX
  const unsigned long half_max_unsigned_long = 2147483647u;  // = 2^31 -1

  if (next >= prev) {
    const unsigned long diff = next - prev;

    if (diff <= half_max_unsigned_long) {
      // Normal situation, just return the difference.
      // Difference is a positive value.
      signed_diff = static_cast<long>(diff);
    } else {
      // prev has overflow, return a negative difference value
      signed_diff = static_cast<long>((UINT32_MAX - next) + prev + 1u);
      signed_diff = -1 * signed_diff;
    }
  } else {
    // next < prev
    const unsigned long diff = prev - next;

    if (diff <= half_max_unsigned_long) {
      // Normal situation, return a negative difference value
      signed_diff = static_cast<long>(diff);
      signed_diff = -1 * signed_diff;
    } else {
      // next has overflow, return a positive difference value
      signed_diff = static_cast<long>((UINT32_MAX - prev) + next + 1u);
    }
  }
  return signed_diff;
}

long timePassedSince2(unsigned long timestamp) {
  return timeDiff(timestamp, millis());
}

bool timeOutReached(unsigned long timer) {
  const long passed = timePassedSince2(timer);

  return passed >= 0;
}

bool isLeapYear(int year) { return LEAP_YEAR(year); }


// from tasmota :

inline int32_t TimeDifference(uint32_t prev, uint32_t next) {
  return ((int32_t)(next - prev));
}

int32_t TimePassedSince(uint32_t timestamp) {
  return TimeDifference(timestamp, millis());
}

bool TimeReached(uint32_t timer) {
  const long passed = TimePassedSince(timer);
  return (passed >= 0);
}

void SetNextTimeInterval(uint32_t& timer, const uint32_t step) {
  timer += step;
  const long passed = TimePassedSince(timer);
  if (passed < 0) {
    return;
  }

  if (static_cast<unsigned long>(passed) > step) {
    timer = millis() + step;
    return;
  }

  timer = millis() + (step - passed);
}

int32_t TimePassedSinceUsec(uint32_t timestamp) {
  return TimeDifference(timestamp, micros());
}

bool TimeReachedUsec(uint32_t timer) {
  const long passed = TimePassedSinceUsec(timer);
  return (passed >= 0);
}

} // namespace helper_time
