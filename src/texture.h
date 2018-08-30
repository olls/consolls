#pragma once

#include "allocate.h"
#include "types.h"


namespace Texture
{

struct Texture
{
  u32 width;
  u32 height;
  u32 *pixels;
};


bool
init(Texture& texture, s32 width, s32 height);


bool
set_pixel(Texture& texture, u32 x, u32 y, u32 colour);


void
destroy(Texture& texture);

} // namespace Texture