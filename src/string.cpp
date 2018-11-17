#include "string.h"


namespace String
{

bool
starts_with(String const & string, String const & substring)
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
equals(String const & a, String const & b)
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
all(String const & string, bool(*condition)(char))
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