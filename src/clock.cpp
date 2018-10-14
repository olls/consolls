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
  clock.accumulated_frame_dt += frame_dt;

  if (CLOCK_RATE_US > frame_dt)
  {
    Time::sleep_us(CLOCK_RATE_US - frame_dt);
  }

  clock.frame_count += 1;
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
print_report(Clock const & clock)
{
  printf("Frame overruns: %u / %u\n", clock.frame_overruns, clock.frame_count);
  printf("Average frame dt: %f\n", clock.accumulated_frame_dt / (r32)clock.frame_count);
}

} // namespace Clock