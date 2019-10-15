#pragma once

#include "utils/mem.h"
#include "utils/types.h"


#define print_s(string) ((s32)(string).length), ((string).start)


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


// Awful SFINAE for String ctor overloads :(
struct Dummy {};
template<typename T> struct IsCharPtr {};
template<> struct IsCharPtr<const char *> { typedef Dummy* Type; };
template<> struct IsCharPtr<char *> { typedef Dummy* Type; };


struct String
{
  char const* start;
  u32 length;

  String() = default;

  // constexpr ctor templating to only match on string literals.
  template <size_t N>
  constexpr
  String(char const (&_start)[N]):
    start(_start),
    length(N-1)
  {}

  template <typename T>
  String(T _start, typename IsCharPtr<T>::Type=0):
    start(_start),
    length((u32)strlen(_start))
  {}

  constexpr
  String(char const* _start, u32 _length):
    start(_start),
    length(_length)
  {}
};


String
format(char const * format...)  __attribute__((format(printf, 1, 2)));


String
cat(String const& a, String const& b);


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
