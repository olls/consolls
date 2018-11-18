#include "string-array.h"
#include "types.h"


namespace StringArray
{

void
print(StringArray strings)
{
  for (u32 string_index = 0;
       string_index < strings.n_elements;
       ++string_index)
  {
    String::String string = strings[string_index];

    printf("%.*s", print_s(string));
  }
}

} // namespace StringArray