#include "utils/types.h"
#include "machine/machine.h"

namespace Socket
{

bool
init(bool receive);


bool
advance(bool receive, Machine::Machine& machine);


void
destroy();

} // namespace Socket