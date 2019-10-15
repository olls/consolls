#pragma once

#include "utils/types.h"


namespace Palette
{

using Colour = u32;


static Colour const COLOURS[] = {
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
  White,       // 0x0
  Yellow,      // 0x1
  LightPink,   // 0x2
  LightGrey,   // 0x3
  Pink,        // 0x4
  LightBlue,   // 0x5
  Gold,        // 0x6
  LightBrown,  // 0x7
  Red,         // 0x8
  Cyan,        // 0x9
  Mauve,       // 0xa
  Grey,        // 0xb
  DarkRed,     // 0xc
  Blue,        // 0xd
  Brown,       // 0xe
  Black        // 0xf
} Index;


inline
Colour
get_colour(Index index)
{
  Colour result = COLOURS[index];
  return result;
}

} // namespace Palette
