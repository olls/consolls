#pragma once

#include "utils/allocate.h"
#include "utils/assert.h"
#include "utils/types.h"


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


inline void
set_pixel(Texture& texture, u32 x, u32 y, Pixel colour)
{
  assert(x < texture.width &&
         y < texture.height);
  texture.pixels[texture.width*y + x] = colour;
}


void
destroy(Texture& texture);

} // namespace Texture