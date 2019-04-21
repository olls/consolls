#include "allocate.h"
#include "string-array.h"
#include "types.h"


namespace StringArray
{

String::String
concat(StringArray const & strings)
{
  String::String result = {};

  for (u32 string_index = 0;
       string_index < strings.n_elements;
       ++string_index)
  {
    String::String const& string = strings[string_index];
    result.length += string.length;
  }

  char* offset = Allocate::allocate<char>(result.length);
  result.start = offset;

  for (u32 string_index = 0;
       string_index < strings.n_elements;
       ++string_index)
  {
    String::String const& string = strings[string_index];
    strncpy(offset, string.start, string.length);
    offset += string.length;
  }
  return result;
}


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