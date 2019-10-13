#include "number-parser.h"

namespace NumberParser
{

bool
parse_u32(String::String text, u32& result)
{
  bool success = true;

  result = 0;

  for (u32 i = 0;
       i < text.length;
       ++i)
  {
    char c = text.start[i];
    if (c >= '0' && c <= '9')
    {
      result *= 10;
      result += c - '0';
    }
    else
    {
      success = false;
      break;
    }
  }

  return success;
}

} // namespace NumberParser