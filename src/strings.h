#pragma once

#include "array.h"
#include "string-array.h"
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
add(Table& strings, String::String string);


String::String&
get(Table const & strings, ID id);


void
string(Table const& strings, StringArray::StringArray& result);

} // namespace Strings