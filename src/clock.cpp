#include "clock.h"

#include "debug.h"
#include "assert.h"

#include <sys/time.h>
#include <unistd.h>


namespace Clock
{

static const u32 CLOCK_RATE_US = 2;


u64
get_us()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return ((u64)tv.tv_sec * (u64)1000000) + (u64)tv.tv_usec;
}


bool
sleep_us(u32 us)
{
  bool success = usleep(us) == 0;
  return success;
}


void
regulate(Clock& clock)
{
  u64 now = get_us();

  if (clock.last_frame_start == 0)
  {
    clock.last_frame_start = now;
  }

  u32 frame_dt = now - clock.last_frame_start;

  if (CLOCK_RATE_US > frame_dt)
  {
    sleep_us(CLOCK_RATE_US - frame_dt);
  }

#ifdef _DEBUG
  if (frame_dt > CLOCK_RATE_US)
  {
    printf("Missing clock rate by %uus  -  Clock took %uus / %uus\n", frame_dt - CLOCK_RATE_US, frame_dt, CLOCK_RATE_US);
  }
#endif

  clock.last_frame_start = get_us();
}

} // namespace Clock