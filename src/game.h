#pragma once

#include "texture.h"
#include "input.h"
#include "frame-id.h"
#include "types.h"


namespace Game
{

struct State
{
  s32 width;
  s32 height;

  Texture::Texture texture;

  FrameID::FrameID frame_id;
  Input::Input input;
};


bool
run();

} // namespace Game