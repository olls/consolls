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


struct BuiltIn
{
  ID add8;
  ID add16;
  ID sub8;
  ID sub16;
  ID put8;
};


ID
add(Identifiers& identifiers, Identifier const& identifier);


ID
find(Identifiers const & identifiers, Strings::ID string);


BuiltIn
init_built_in_identifiers(Identifiers& identifiers, Strings::Table & strings, TypeSystem::Types & types);

} // namespace Identifiers

} // namespace Compolls