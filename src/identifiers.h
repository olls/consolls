#pragma once

#include "type-system.h"
#include "symbols.h"
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
  Symbols::ID symbol;
  TypeSystem::ID type;
};

using Identifiers = Array::Array<Identifier>;


void
init_built_in_identifiers(Identifiers& identifiers, Symbols::Table & symbols, TypeSystem::Types & types, TypeSystem::BuiltInTypes const & built_in_types);


ID
find(Identifiers const & identifiers, Symbols::ID symbol);

} // namespace Identifiers

} // namespace Compolls