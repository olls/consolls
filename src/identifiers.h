#pragma once

#include "type-system.h"
#include "strings.h"
#include "array.h"
#include "types.h"


namespace Compolls
{

namespace Identifiers
{

using ID = u32;
static ID const InvalidID = (u32)-1;


struct Identifier
{
  Strings::ID string;
  TypeSystem::ID type;
};

using Identifiers = Array::Array<Identifier>;


void
init_built_in_identifiers(Identifiers& identifiers, Strings::Table & strings, TypeSystem::Types & types);


ID
find(Identifiers const & identifiers, Strings::ID string);

} // namespace Identifiers

} // namespace Compolls