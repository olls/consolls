#include "clock.h"

#include "time.h"
#include "debug.h"
#include "assert.h"


namespace Clock
{

static const u32 CLOCK_RATE_US = 20;


void
regulate(Clock& clock)
{
  u64 now = Time::get_us();

  if (clock.last_frame_start == 0)
  {
    clock.last_frame_start = now;
  }

  u32 frame_dt = now - clock.last_frame_start;

  if (CLOCK_RATE_US > frame_dt)
  {
    Time::sleep_us(CLOCK_RATE_US - frame_dt);
  }

  if (frame_dt > CLOCK_RATE_US)
  {
    clock.frame_overruns += 1;
#ifdef _DEBUG
    printf("Missing clock rate by %uus  -  Clock took %uus / %uus\n", frame_dt - CLOCK_RATE_US, frame_dt, CLOCK_RATE_US);
#endif
  }

  clock.last_frame_start = Time::get_us();
}


void
print_report(Clock& clock)
{
  printf("Frame overruns: %u\n", clock.frame_overruns);
}

} // namespace Clock