#include "machine.h"

#include "palette.h"
#include "assert.h"

#include <string.h>


namespace Machine
{

void
advance(Machine& machine)
{
  MemoryAddress next_instruction = get<u16>(machine.memory, Reserved::NI);

  static u8 frame = 0;
  ++frame;
  memset(&machine.memory.bytes[Reserved::ScreenBuffer], (((frame/16)%16)&0xf) | (Pallete::Index::Red << 4), machine.memory.screen_buffer_size);
}


void
output_screen_buffer(Machine& machine, Texture::Texture& texture)
{
  u8* sb = &machine.memory.bytes[Reserved::ScreenBuffer];

  u8 pixel_mask = (1 << machine.memory.pixel_size) - 1;

  for (u32 y = 0;
       y < machine.memory.sb_height;
       ++y)
  {
    for (u32 x = 0;
         x < machine.memory.sb_width;
         ++x)
    {
      u32 pixel_offset = (y * machine.memory.sb_width) + x;
      r32 byte_offset_frac = pixel_offset * (machine.memory.pixel_size / 8.0);

      u32 byte_offset = u32(byte_offset_frac);
      assert(byte_offset < machine.memory.screen_buffer_size);

      u8 byte = sb[byte_offset];

      u32 bit_offset = 8*(byte_offset_frac - byte_offset);
      assert(bit_offset < 8);

      Pallete::Index colour_index = (Pallete::Index)((byte >> bit_offset) & pixel_mask);
      Pallete::Colour colour = Pallete::get_colour(colour_index);

      Texture::set_pixel(texture, x, y, colour);
    }
  }
}

} // namespace Machine