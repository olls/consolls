#pragma once

#include "types.h"

#define S(c_string) ((String::String){.length=(u32)strlen(c_string), .start=(c_string)})

#define print_s(string) ((string).length), ((string).start)


namespace String
{

inline bool
is_whitespace(char c)
{
  return (c == ' ' ||
          c == '\t' ||
          c == '\n');
}


inline bool
is_alpha(char c)
{
  return ((c >= 'A' && c <= 'Z') ||
          (c >= 'a' && c <= 'z'));
}


inline bool
is_num(char c)
{
  return (c >= '0' && c <= '9');
}


inline bool
is_alpha_num(char c)
{
  return (is_alpha(c) ||
          is_num(c));
}


struct String
{
  u32 length;
  char const * start;
};


String
string_f(char const * format...)  __attribute__((format(printf, 1, 2)));


bool
starts_with(String const & string, String const & substring);


bool
equals(String const & a, String const & b);


bool
all(String const & string, bool(*condition)(char));

} // namespace String