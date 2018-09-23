#pragma once

#include "types.h"


namespace Clock
{

struct Clock
{
  u32 frame_overruns;
  u64 last_frame_start;
};


void
regulate(Clock& clock);


void
print_report(Clock& clock);

} // namespace Clock