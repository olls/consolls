#include "machine.h"

#include "palette.h"
#include "assert.h"


namespace Machine
{

template <typename width>
width
get_from_address(Machine& machine, MemoryAddress& addr_addr)
{
  MemoryAddress addr = advance_addr<MemoryAddress>(machine, addr_addr);
  width value = get<width>(machine, addr);
  return value;
}


template <typename width>
void
add_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  width in_a = get_from_address<width>(machine, instruction_ptr);
  width in_b = get_from_address<width>(machine, instruction_ptr);
  MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

  width result = in_a + in_b;

  set<width>(machine, result_ptr, result);
}


template <typename width>
void
sub_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  width in_a = get_from_address<width>(machine, instruction_ptr);
  width in_b = get_from_address<width>(machine, instruction_ptr);
  MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

  width result = in_a - in_b;

  set<width>(machine, result_ptr, result);
}


template <typename width>
void
mul_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  width in_a = get_from_address<width>(machine, instruction_ptr);
  width in_b = get_from_address<width>(machine, instruction_ptr);
  MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

  width result = in_a * in_b;

  set<width>(machine, result_ptr, result);
}


template <typename width>
void
div_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  width in_a = get_from_address<width>(machine, instruction_ptr);
  width in_b = get_from_address<width>(machine, instruction_ptr);
  MemoryAddress result_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);

  width result = in_a / in_b;

  set<width>(machine, result_ptr, result);
}


template <typename width>
void
cmp_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  width in_a = get_from_address<width>(machine, instruction_ptr);
  width in_b = get_from_address<width>(machine, instruction_ptr);
  MemoryAddress then = get<MemoryAddress>(machine, instruction_ptr);

  if (in_a > in_b)
  {
    instruction_ptr = then;
  }
}


template <typename width>
void
set_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  MemoryAddress to_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
  width value = advance_addr<width>(machine, instruction_ptr);

  set<width>(machine, to_ptr, value);
}


template <typename width>
void
copy_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  MemoryAddress from = get_from_address<MemoryAddress>(machine, instruction_ptr);
  MemoryAddress to = get_from_address<MemoryAddress>(machine, instruction_ptr);

  width from_value = get<width>(machine, from);
  set<width>(machine, to, from_value);
}


void
advance(Machine& machine)
{
  MemoryAddress instruction_ptr = get<MemoryAddress>(machine, Reserved::NI);
  InstructionCode instruction = advance_addr<InstructionCode>(machine, instruction_ptr);

  assert(instruction_ptr < Reserved::ScreenBuffer);

  switch (instruction)
  {
    case (InstructionCode::NOP):
      break;

    case (InstructionCode::ADD):    add_inst<u8>(machine, instruction_ptr);
      break;
    case (InstructionCode::ADD_W):  add_inst<u16>(machine, instruction_ptr);
      break;

    case (InstructionCode::SUB):    sub_inst<u8>(machine, instruction_ptr);
      break;
    case (InstructionCode::SUB_W):  sub_inst<u16>(machine, instruction_ptr);
      break;

    case (InstructionCode::MUL):    mul_inst<u8>(machine, instruction_ptr);
      break;
    case (InstructionCode::MUL_W):  mul_inst<u16>(machine, instruction_ptr);
      break;

    case (InstructionCode::DIV):    div_inst<u8>(machine, instruction_ptr);
      break;
    case (InstructionCode::DIV_W):  div_inst<u16>(machine, instruction_ptr);
      break;

    case (InstructionCode::JUMP):
    {
      instruction_ptr = advance_addr<MemoryAddress>(machine, instruction_ptr);
    } break;

    case (InstructionCode::CMP):    cmp_inst<u8>(machine, instruction_ptr);
      break;
    case (InstructionCode::CMP_W):  cmp_inst<u16>(machine, instruction_ptr);
      break;

    case (InstructionCode::SET):    set_inst<u8>(machine, instruction_ptr);
      break;
    case (InstructionCode::SET_W):  set_inst<u16>(machine, instruction_ptr);
      break;

    case (InstructionCode::COPY):   copy_inst<u8>(machine, instruction_ptr);
      break;
    case (InstructionCode::COPY_W): copy_inst<u16>(machine, instruction_ptr);
      break;
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