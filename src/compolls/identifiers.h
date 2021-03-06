#pragma once

#include "type-system.h"
#include "strings.h"

#include "utils/array.h"
#include "utils/types.h"


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


struct BuiltIn
{
  ID add8;
  ID add16;
  ID sub8;
  ID sub16;
  ID mul8;
  ID mul16;
  ID div8;
  ID div16;
  ID put8;
};


ID
add(Identifiers& identifiers, Identifier const& identifier);


ID
find(Identifiers const& identifiers, Strings::ID string);


BuiltIn
init_built_in_identifiers(Identifiers& identifiers, Strings::Table & strings, TypeSystem::Types & types);

} // namespace Identifiers

} // namespace Compolls
