#include "machine.h"


namespace Machine
{

void
advance(Machine& machine)
{
  MemoryAddress next_instruction = get<u16>(machine.memory, Reserved::NI);
}

} // namespace Machine