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
      result = i;
      break;
    }
  }

  if (result == -1)
  {
    result = strings.table.n_elements;
    Array::add(strings.table, string);
    // printf("String: \"%.*s\" %d\n", print_s(string), result);
  }

  return result;
}


String::String &
get(Table const & strings, ID id)
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