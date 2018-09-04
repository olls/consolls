#pragma once

#include "machine.h"
#include "texture.h"
#include "input.h"
#include "frame-id.h"
#include "types.h"


namespace Game
{

struct State
{
  Texture::Texture texture;

  FrameID::FrameID frame_id;
  Input::Input input;

  Machine::Machine machine;
};


bool
run();

} // namespace Game