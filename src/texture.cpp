#include "texture.h"
#include "debug.h"


namespace Texture
{

bool
init(Texture& texture, s32 width, s32 height)
{
  bool success = true;

  texture.width = width;
  texture.height = height;
  texture.pixels = Allocate::allocate<u32>(texture.width * texture.height);
  if (texture.pixels == NULL)
  {
    printf("Failed to allocate texture screen pixels.\n");
    success &= false;
  }

  return success;
}


bool
set_pixel(Texture& texture, u32 x, u32 y, u32 colour)
{
  bool success = true;

  if (x < texture.width &&
      y < texture.height)
  {
    texture.pixels[texture.width*y + x] = colour;
  }
  else
  {
    success &= false;
  }

  return success;
}


void
destroy(Texture& texture)
{
  Allocate::unallocate(texture.pixels);
}

} // namespace Texture