#pragma once

#include "utils/types.h"

#include <sys/time.h>
#include <unistd.h>


namespace Time
{

inline
u64
get_us()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return ((u64)tv.tv_sec * (u64)1000000) + (u64)tv.tv_usec;
}


inline
bool
sleep_us(u32 us)
{
  bool success = usleep(us) == 0;
  return success;
}

} // namespace Time
