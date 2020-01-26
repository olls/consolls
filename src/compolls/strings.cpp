#include "strings.h"


namespace Strings
{

ID
add(Table& strings, String::String string)
{
  ID result = -1;

  for (u32 i = 0;
       i < strings.table.n_elements;
       ++i)
  {
    if (String::equals(strings.table[i], string))
    {
      result = (s32)i;
      break;
    }
  }

  if (result == -1)
  {
    result = (s32)strings.table.n_elements;
    Array::add(strings.table, string);
  }

  return result;
}


String::String const&
get(Table const& strings, ID id)
{
  return strings.table[id];
}

String::String&
get(Table& strings, ID id)
{
  return strings.table[id];
}


void
string(Table const& strings, StringArray::StringArray& result)
{
  for (u32 index = 0;
       index < strings.table.n_elements;
       ++index)
  {
    String::String const& string = strings.table[index];
    result += String::format("%u: \"", index);
    result += string;
    result += "\"\n";
  }
}

} // namespace Strings
