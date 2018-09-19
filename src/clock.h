#pragma once

#include "types.h"


namespace Clock
{

struct Clock
{
  u64 last_frame_start;
};


void
regulate(Clock& clock);

} // namespace Clock