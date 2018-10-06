#pragma once

#include "machine.h"
#include "texture.h"
#include "input.h"
#include "frame-id.h"
#include "clock.h"
#include "timer.h"
#include "types.h"


namespace Game
{

struct State
{
  Texture::Texture texture;

  FrameID::FrameID frame_id;
  Input::Input input;
  Timer::Timer input_update;
  Clock::Clock clock;

  Machine::Machine machine;
};


bool
run(bool debugger);

} // namespace Game