#pragma once

#include "array.h"
#include "string.h"
#include "types.h"


namespace Compolls
{

namespace Symbols
{

using ID = s32;


struct Table
{
  Array::Array<String::String> table;
};


ID
add(Table& symbols, String::String symbol);


String::String&
get(Table const & symbols, ID id);

} // namespace Symbols

} // namespace Compolls