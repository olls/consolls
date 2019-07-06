#include "machine-serialisation.h"

#include "file.h"


namespace MachineSerialisation
{

bool
serialise(char const* filename, Machine::Machine const& machine)
{
  bool success = true;

  File::File out = {};
  if (!File::open(filename, &out, true, machine.memory.size))
  {
    success &= false;
  }
  else
  {
    memcpy(out.write_ptr, machine.memory.bytes, machine.memory.size);
    File::close(&out);
    printf("Written to output file \"%s\".\n", filename);
  }

  return success;
}


bool
deserialise(char const* filename, Machine::Machine& machine)
{
  bool success = true;

  File::File file = {};
  if (!File::open(filename, &file))
  {
    success &= false;
  }
  else
  {
    if (file.size != machine.memory.size)
    {
      printf("\"%s\" is not the correct size (%u != %u).\n", filename, file.size, machine.memory.size);
      success &= false;
    }
    else
    {
      memcpy(machine.memory.bytes, file.read_ptr, machine.memory.size);
      File::close(&file);
      printf("Machine loaded from file \"%s\".\n", filename);
    }
  }

  return success;
}

}  // namespace MachineSerialisation
