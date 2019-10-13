#pragma once

#include "machine/machine.h"
#include "machine/texture.h"
#include "input.h"
#include "frame-id.h"
#include "clock.h"
#include "timer.h"
#include "utils/types.h"
#include "loader/options.h"


namespace Game
{

struct State
{
  Texture::Texture texture;

  FrameID::FrameID frame_id;
  Input::Input input;
  Timer::Timer input_update;
  Clock::Clock clock;

  bool step_mode;
  bool stepping;

  Machine::Machine machine;
};


bool
run(Options::Args args);

} // namespace Game