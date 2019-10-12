#pragma once

#include "types.h"

#include <string.h>

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


inline bool
is_alpha_num_underscore(char c)
{
  return (is_alpha_num(c) ||
          c == '_');
}


struct String
{
  u32 length;
  char const * start;

  String() = default;

  String(char const * start)
  {
    this->start = start;
    this->length = strlen(start);
  }

  String(char const * start, u32 length)
  {
    this->start = start;
    this->length = length;
  }
};


String
format(char const * format...)  __attribute__((format(printf, 1, 2)));


bool
starts_with(String const & string, String const & substring);


bool
equals(String const & a, String const & b);


bool
all(String const & string, bool(*condition)(char));


inline
String
sub_string(String const& text, u32 start, u32 end)
{
  String result(text.start + start, end - start);
  return result;
}

} // namespace String