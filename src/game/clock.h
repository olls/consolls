#pragma once

#include "utils/types.h"


namespace Clock
{

struct Clock
{
  u32 n_instructions;
  u64 last_reset_us;
};


void
regulate(Clock& clock);

} // namespace Clock