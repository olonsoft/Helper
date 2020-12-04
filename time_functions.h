#pragma once

#define SECS_PER_MIN ((uint32_t)(60UL))
#define SECS_PER_HOUR ((uint32_t)(3600UL))
#define SECS_PER_DAY ((uint32_t)(SECS_PER_HOUR * 24UL))
#define MINS_PER_HOUR ((uint32_t)(60UL))
#define LEAP_YEAR(Y)                        \
  (((1970 + Y) > 0) && !((1970 + Y) % 4) && \
   (((1970 + Y) % 100) || !((1970 + Y) % 400)))

  uint32_t getUpTimeSeconds();
  String getUpTimeString();
  String hourToString(time_t time1);
  String getLocalHourString();
  String timeToString();
  long ICACHE_RAM_ATTR timeDiff(const unsigned long prev,
                                const unsigned long next);
  long timePassedSince(unsigned long timestamp);
  bool timeOutReached(unsigned long timer);
  bool isLeapYear(int year);
