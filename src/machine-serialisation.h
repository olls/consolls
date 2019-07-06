#include "machine.h"
#include "strings.h"
#include "types.h"


namespace MachineSerialisation
{

bool
serialise(char const* filename, Machine::Machine const& machine);


bool
deserialise(char const* filename, Machine::Machine& machine);

}  // namespace MachineSerialisation
