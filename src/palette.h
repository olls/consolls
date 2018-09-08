#pragma once

#include "types.h"


namespace Palette
{

using Colour = u32;


static const Colour COLOURS[] = {
  0xF2F1DA00, // White
  0xF2EC7900, // Yellow
  0xFFCCCE00, // Light Pink
  0xDDD7D700, // Light Grey
  0xFF999D00, // Pink
  0xAABBDD00, // Light BLue
  0x99882200, // Gold
  0x7F7C3F00, // Light Brown
  0xF21C2600, // Red
  0x11888800, // Cyan
  0x55557700, // Mauve
  0x4C3D3D00, // Grey
  0x66000000, // Dark Red
  0x11228800, // Blue
  0x22220000, // Brown
  0x11111000  // Black
};


typedef enum : u8
{
  White,
  Yellow,
  LightPink,
  LightGrey,
  Pink,
  LightBlue,
  Gold,
  LightBrown,
  Red,
  Cyan,
  Mauve,
  Grey,
  DarkRed,
  Blue,
  Brown,
  Black
} Index;


inline Colour
get_colour(Index index)
{
  Colour result = COLOURS[index];
  return result;
}

} // namespace Palette