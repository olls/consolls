#include "machine.h"


namespace Machine
{

void
execute(Machine& machine)
{
  // TODO: Implement get_u16
  MemoryAddress next_instruction = 0;
  *(u8*)(&next_instruction) = get_byte(machine.memory, Registers::NI);
  *((u8*)(&next_instruction)+1) = get_byte(machine.memory, Registers::NI+1);
}

} // namespace Machine