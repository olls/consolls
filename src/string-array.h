#pragma once

#include "string.h"
#include "array.h"


namespace StringArray
{

using StringArray = Array::Array<String::String>;


void
print(StringArray strings);

} // namespace StringArray