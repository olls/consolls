#include "machine.h"

#include "palette.h"
#include "instructions.h"
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
  Instructions::ADD args = advance_addr<Instructions::ADD>(machine, instruction_ptr);

  width a = get<width>(machine, args.a);
  width b = get<width>(machine, args.b);
  width result = a + b;

  set<width>(machine, args.result, result);
}


template <typename width>
void
sub_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  Instructions::SUB args = advance_addr<Instructions::SUB>(machine, instruction_ptr);

  width a = get<width>(machine, args.a);
  width b = get<width>(machine, args.b);
  width result = a - b;

  set<width>(machine, args.result, result);
}


template <typename width>
void
mul_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  Instructions::MUL args = advance_addr<Instructions::MUL>(machine, instruction_ptr);

  width a = get<width>(machine, args.a);
  width b = get<width>(machine, args.b);
  width result = a * b;

  set<width>(machine, args.result, result);
}


template <typename width>
void
div_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  Instructions::DIV args = advance_addr<Instructions::DIV>(machine, instruction_ptr);

  width a = get<width>(machine, args.a);
  width b = get<width>(machine, args.b);
  width result = a / b;

  set<width>(machine, args.result, result);
}


void
jump_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  Instructions::JUMP args = advance_addr<Instructions::JUMP>(machine, instruction_ptr);
  instruction_ptr = args.addr;
}


template <typename width>
void
cmp_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  Instructions::CMP args = advance_addr<Instructions::CMP>(machine, instruction_ptr);

  width a = get<width>(machine, args.a);
  width b = get<width>(machine, args.b);

  if (a > b)
  {
    instruction_ptr = args.addr;
  }
}


template <typename width>
void
set_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  Instructions::SET args = advance_addr<Instructions::SET<width>>(machine, instruction_ptr);

  set<width>(machine, args.addr, args.value);
}


template <typename width>
void
copy_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  Instructions::COPY args = advance_addr<Instructions::COPY>(machine, instruction_ptr);

  MemoryAddress from = get<MemoryAddress>(machine, args.from);
  MemoryAddress to = get<MemoryAddress>(machine, args.to);

  width from_value = get<width>(machine, from);
  set<width>(machine, to, from_value);
}


void
advance(Machine& machine)
{
  MemoryAddress instruction_ptr = get<MemoryAddress>(machine, Reserved::NI);
  Instructions::Code instruction = advance_addr<Instructions::Code>(machine, instruction_ptr);

  assert(instruction_ptr < Reserved::ScreenBuffer);

  switch (instruction)
  {
    case (Instructions::Code::NOP):
      break;

    case (Instructions::Code::ADD):    add_inst<u8>(machine, instruction_ptr);
      break;
    case (Instructions::Code::ADD_W):  add_inst<u16>(machine, instruction_ptr);
      break;

    case (Instructions::Code::SUB):    sub_inst<u8>(machine, instruction_ptr);
      break;
    case (Instructions::Code::SUB_W):  sub_inst<u16>(machine, instruction_ptr);
      break;

    case (Instructions::Code::MUL):    mul_inst<u8>(machine, instruction_ptr);
      break;
    case (Instructions::Code::MUL_W):  mul_inst<u16>(machine, instruction_ptr);
      break;

    case (Instructions::Code::DIV):    div_inst<u8>(machine, instruction_ptr);
      break;
    case (Instructions::Code::DIV_W):  div_inst<u16>(machine, instruction_ptr);
      break;

    case (Instructions::Code::JUMP):   jump_inst(machine, instruction_ptr);
      break;

    case (Instructions::Code::CMP):    cmp_inst<u8>(machine, instruction_ptr);
      break;
    case (Instructions::Code::CMP_W):  cmp_inst<u16>(machine, instruction_ptr);
      break;

    case (Instructions::Code::SET):    set_inst<u8>(machine, instruction_ptr);
      break;
    case (Instructions::Code::SET_W):  set_inst<u16>(machine, instruction_ptr);
      break;

    case (Instructions::Code::COPY):   copy_inst<u8>(machine, instruction_ptr);
      break;
    case (Instructions::Code::COPY_W): copy_inst<u16>(machine, instruction_ptr);
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