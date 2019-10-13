#pragma once

#include "utils/allocate.h"
#include "utils/debug.h"
#include "utils/assert.h"
#include "utils/types.h"


namespace Texture
{

template <typename Pixel_t>
struct Texture
{
  using Pixel = Pixel_t;

  u32 width;
  u32 height;

  Pixel *pixels;
};


template <typename Pixel>
bool
allocate(Texture<Pixel>& texture, s32 width, s32 height)
{
  bool success = true;

  texture.width = width;
  texture.height = height;
  texture.pixels = Allocate::allocate<Pixel>(texture.width * texture.height);
  if (texture.pixels == NULL)
  {
    printf("Failed to allocate texture screen pixels.\n");
    success &= false;
  }

  return success;
}


template <typename Pixel>
void
set_pixel(Texture<Pixel>& texture, u32 x, u32 y, Pixel colour)
{
  assert(x < texture.width &&
         y < texture.height);
  texture.pixels[texture.width*y + x] = colour;
}


template <typename Pixel>
void
destroy(Texture<Pixel>& texture)
{
  Allocate::unallocate(texture.pixels);
}

} // namespace Texture