#pragma once

#include "frame-id.h"
#include "input.h"

#include "utils/types.h"


namespace SDL_Events
{

bool
advance(Input::Input& input, FrameID::FrameID frame_id);

}  // namespace SDL_Events
