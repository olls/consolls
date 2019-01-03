#pragma once

#include "string.h"
#include "types.h"


namespace NumberParser
{

bool
parse_u32(String::String text, u32& result);

} // namespace NumberParser