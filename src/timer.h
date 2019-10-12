#pragma once

#include "time.h"
#include "types.h"


namespace Timer
{

struct Timer
{
  u32 duration_us;
  u64 end;
};


inline void
start(Timer& timer)
{
  u64 now = Time::get_us();
  timer.end = now + timer.duration_us;
}


inline void
init(Timer& timer, u32 duration_us)
{
  timer.duration_us = duration_us;
  start(timer);
}


inline bool
check(Timer& timer)
{
  u64 now = Time::get_us();
  bool result = (now >= timer.end);
  if (result)
  {
    start(timer);
  }
  return result;
}

} // namespace Timer