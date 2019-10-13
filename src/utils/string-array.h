#pragma once

#include "string.h"
#include "array.h"


namespace StringArray
{

using StringArray = Array::Array<String::String>;


String::String
concat(StringArray const & strings);


void
print(StringArray strings);

} // namespace StringArray