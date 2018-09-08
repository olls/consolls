#include "machine.h"

#include "palette.h"
#include "assert.h"


namespace Machine
{

void
advance(Machine& machine)
{
  MemoryAddress instruction_ptr = get<MemoryAddress>(machine, Reserved::NI);
  InstructionCode instruction = advance_addr<InstructionCode>(machine, instruction_ptr);

  assert(instruction_ptr < Reserved::ScreenBuffer);

  switch (instruction)
  {
    case (InstructionCode::NOP):
    {} break;
    case (InstructionCode::ADD):
    {
      MemoryAddress in_a_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress in_b_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

      u8 in_a = get<u8>(machine, in_a_ptr);
      u8 in_b = get<u8>(machine, in_b_ptr);

      u8 result = in_a + in_b;

      set<u8>(machine, result_ptr, result);
    } break;
    case (InstructionCode::ADD_W):
    {
      MemoryAddress in_a_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress in_b_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

      u16 in_a = get<u16>(machine, in_a_ptr);
      u16 in_b = get<u16>(machine, in_b_ptr);

      u16 result = in_a + in_b;

      set<u16>(machine, result_ptr, result);
    } break;
    case (InstructionCode::SUB):
    {
      MemoryAddress in_a_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress in_b_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

      u8 in_a = get<u8>(machine, in_a_ptr);
      u8 in_b = get<u8>(machine, in_b_ptr);

      u8 result = in_a - in_b;

      set<u8>(machine, result_ptr, result);
    } break;
    case (InstructionCode::MUL):
    {
      MemoryAddress in_a_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress in_b_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

      u8 in_a = get<u8>(machine, in_a_ptr);
      u8 in_b = get<u8>(machine, in_b_ptr);

      u8 result = in_a * in_b;

      set<u8>(machine, result_ptr, result);
    } break;
    case (InstructionCode::DIV):
    {
      MemoryAddress in_a_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress in_b_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

      u8 in_a = get<u8>(machine, in_a_ptr);
      u8 in_b = get<u8>(machine, in_b_ptr);

      u8 result = in_a / in_b;

      set<u8>(machine, result_ptr, result);
    } break;
    case (InstructionCode::JUMP):
    {
      MemoryAddress in_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      instruction_ptr = in_ptr;
    } break;
    case (InstructionCode::IF_ZERO):
    {
      MemoryAddress in_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress then_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

      u8 in = get<u8>(machine, in_ptr);

      if (in == 0)
      {
        instruction_ptr = then_ptr;
      }
    } break;
    case (InstructionCode::SET):
    {
      MemoryAddress to_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      u8 value = advance_addr<u8>(machine, instruction_ptr);

      set<u8>(machine, to_ptr, value);
    } break;
    case (InstructionCode::SET_W):
    {
      MemoryAddress to_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress value = advance_addr<MemoryAddress>(machine, instruction_ptr);

      set<MemoryAddress>(machine, to_ptr, value);
    } break;
    case (InstructionCode::COPY):
    {
      MemoryAddress from_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
      MemoryAddress to_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

      MemoryAddress from = get<MemoryAddress>(machine, from_ptr);
      MemoryAddress to = get<MemoryAddress>(machine, to_ptr);

      u8 from_value = get<u8>(machine, from);
      set<u8>(machine, to, from_value);
    } break;
  }

  set<MemoryAddress>(machine, Reserved::NI, instruction_ptr);
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

      Palette::Index colour_index = (Palette::Index)((byte >> bit_offset) & pixel_mask);
      Palette::Colour colour = Palette::get_colour(colour_index);

      Texture::set_pixel(texture, x, y, colour);
    }
  }
}

} // namespace Machine