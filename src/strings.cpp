#include "strings.h"


namespace Strings
{

ID
add(Table& strings, String::String symbol)
{
  ID result = -1;

  for (u32 i = 0;
       i < strings.table.n_elements;
       ++i)
  {
    if (String::equals(strings.table[i], symbol))
    {
      result = i;
      break;
    }
  }

  if (result == -1)
  {
    result = strings.table.n_elements;
    Array::add(strings.table, symbol);
    printf("Symbol: \"%.*s\" %d\n", print_s(symbol), result);
  }

  return result;
}


String::String &
get(Table const & strings, ID id)
{
  return strings.table[id];
}

} // namespace Strings