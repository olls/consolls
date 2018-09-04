#pragma once

#include "allocate.h"
#include "types.h"


namespace Texture
{

using Pixel = u32;


struct Texture
{
  u32 width;
  u32 height;

  Pixel *pixels;
};


bool
allocate(Texture& texture, s32 width, s32 height);


bool
set_pixel(Texture& texture, u32 x, u32 y, u32 colour);


void
destroy(Texture& texture);

} // namespace Texture