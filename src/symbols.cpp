#include "symbols.h"

namespace Compolls
{

namespace Symbols
{

ID
add(Table& symbols, String::String symbol)
{
  ID result = -1;

  for (u32 i = 0;
       i < symbols.table.n_elements;
       ++i)
  {
    if (String::equals(symbols.table[i], symbol))
    {
      result = i;
      break;
    }
  }

  if (result == -1)
  {
    result = symbols.table.n_elements;
    Array::add(symbols.table, symbol);
    printf("Symbol: \"%.*s\" %d\n", print_s(symbol), result);
  }

  return result;
}


String::String &
get(Table const & symbols, ID id)
{
  return symbols.table[id];
}

} // namespace Symbols

} // namespace Compolls