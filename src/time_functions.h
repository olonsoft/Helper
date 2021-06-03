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
  long IRAM_ATTR timeDiff(const unsigned long prev,
                                const unsigned long next);
  long timePassedSince(unsigned long timestamp);
  bool timeOutReached(unsigned long timer);
  bool isLeapYear(int year);

// from tasmota
inline int32_t TimeDifference(uint32_t prev, uint32_t next)
{
  return ((int32_t) (next - prev));
}

int32_t TimePassedSince(uint32_t timestamp)
{
  return TimeDifference(timestamp, millis());
}

bool TimeReached(uint32_t timer)
{
  const long passed = TimePassedSince(timer);
  return (passed >= 0);
}

void SetNextTimeInterval(uint32_t& timer, const uint32_t step)
{
  timer += step;
  const long passed = TimePassedSince(timer);
  if (passed < 0) { return; }

  if (static_cast<unsigned long>(passed) > step) {

    timer = millis() + step;
    return;
  }

  timer = millis() + (step - passed);
}

int32_t TimePassedSinceUsec(uint32_t timestamp)
{
  return TimeDifference(timestamp, micros());
}

bool TimeReachedUsec(uint32_t timer)
{
  const long passed = TimePassedSinceUsec(timer);
  return (passed >= 0);
}