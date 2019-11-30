#include "string.h"

#include "utils/allocate.h"
#include "utils/assert.h"

#include <stdarg.h>


namespace String
{

String
format(char const* format...)
{
  String result = {};

  va_list args, args2;
  va_start(args, format);
  va_copy(args2, args);

  char* buffer = 0;

  s32 length = vsnprintf(NULL, 0, format, args);
  if (length >= 0)
  {
    buffer = Allocate::allocate<char>((u32)length);

    if (vsprintf(buffer, format, args2) == length)
    {
      result.start = buffer;
      result.length = (u32)length;
    }
    else
    {
      assert(0);
    }
  }

  va_end(args);

  return result;
}


String
cat(String const& a, String const& b)
{
  String result;
  result.length = a.length + b.length;
  char* start = Allocate::allocate<char>(result.length);
  memcpy(start, a.start, a.length);
  memcpy(start + a.length, b.start, b.length);
  result.start = start;
  return result;
}


bool
starts_with(String const& string, String const& substring)
{
  bool result = true;

  for (u32 character_index = 0;
       character_index < substring.length;
       ++character_index)
  {
    if (character_index == string.length ||
        substring.start[character_index] != string.start[character_index])
    {
      result = false;
      break;
    }
  }

  return result;
}


bool
equals(String const& a, String const& b)
{
  bool result = true;

  if (a.length != b.length)
  {
    result = false;
  }
  else
  {
    result = starts_with(a, b);
  }

  return result;
}


bool
all(String const& string, bool(*condition)(char))
{
  bool result = true;

  for (u32 character_index = 0;
       character_index < string.length;
       ++character_index)
  {
    result &= condition(string.start[character_index]);
    if (!result)
    {
      break;
    }
  }

  return result;
}

} // namespace String
