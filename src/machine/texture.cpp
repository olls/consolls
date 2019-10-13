#include "texture.h"
#include "utils/debug.h"


namespace Texture
{

bool
allocate(Texture& texture, s32 width, s32 height)
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


void
destroy(Texture& texture)
{
  Allocate::unallocate(texture.pixels);
}

} // namespace Texture