#pragma once

#include "utils/array.h"
#include "utils/string-array.h"
#include "utils/string.h"
#include "utils/types.h"


namespace Strings
{

using ID = s32;


struct Table
{
  Array::Array<String::String> table;
};


ID
add(Table& strings, String::String string);


String::String const&
get(Table const& strings, ID id);

String::String&
get(Table& strings, ID id);


void
string(Table const& strings, StringArray::StringArray& result);

} // namespace Strings
