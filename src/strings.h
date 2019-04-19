#pragma once

#include "array.h"
#include "string.h"
#include "types.h"


namespace Strings
{

using ID = s32;


struct Table
{
  Array::Array<String::String> table;
};


ID
add(Table& strings, String::String symbol);


String::String&
get(Table const & strings, ID id);

} // namespace Strings