#include "clock.h"

#include "time.h"
#include "debug.h"
#include "assert.h"


namespace Clock
{

static const u32 CLOCK_RATE_NS = 5000u;


void
regulate(Clock& clock)
{
  clock.n_instructions += 1;
  if (clock.n_instructions > 1000u / CLOCK_RATE_NS)
  {
    u32 time_needed_us = (clock.n_instructions * CLOCK_RATE_NS) / 1000u;
    u64 now_us = Time::get_us();

    u32 time_taken_us = now_us - clock.last_reset_us;

    clock.last_reset_us = now_us;
    clock.n_instructions = 0;

    if (time_taken_us < time_needed_us)
    {
      Time::sleep_us(time_needed_us - time_taken_us);
    }
  }
}

} // namespace Clock