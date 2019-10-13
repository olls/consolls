#pragma once

#include "input.h"
#include "frame-id.h"
#include "clock.h"
#include "timer.h"

#include "loader/options.h"

#include "machine/machine.h"
#include "machine/palette.h"

#include "utils/texture.h"
#include "utils/types.h"


namespace Game
{

using TextureType = Texture::Texture<Palette::Colour>;


struct State
{
  TextureType texture;

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