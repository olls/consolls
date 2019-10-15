#pragma once

#include "utils/types.h"


namespace FrameID
{

using FrameID = u32;


inline
void
update(FrameID& frame_id)
{
  frame_id += 1;
}

} // namespace FrameID
