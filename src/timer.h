#pragma once

#include "time.h"
#include "types.h"


namespace Timer
{

struct Timer
{
  u32 duration;
  u64 end;
};


inline void
start(Timer& timer)
{
  u64 now = Time::get_us();
  timer.end = now + timer.duration;
}


inline void
init(Timer& timer, u32 duration)
{
  timer.duration = duration;
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