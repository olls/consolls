#pragma once

#include "strings.h"

#include "utils/string.h"
#include "utils/array.h"
#include "utils/types.h"


namespace Tokeniser
{

struct Token
{
  u32 start;
  u32 end;
  Strings::ID string;
};


Array::Array<Token>
tokenise(String::String text, Strings::Table& strings);


// Deprecated
inline
String::String
string(String::String const& text, Token const& token)
{
  return String::sub_string(text, token.start, token.end);
}


inline
String::String
string(Strings::Table const& strings, Token const& token)
{
  return Strings::get(strings, token.string);
}


inline
void
print(Strings::Table const& strings, Array::Array<Token> const& tokens, u32 start_pos = 0)
{
  for (u32 token_index = start_pos;
       token_index < tokens.n_elements;
       ++token_index)
  {
    if (token_index != start_pos)
    {
      printf(", ");
    }
    printf("\"%.*s\"", print_s(string(strings, tokens[token_index])));
  }
  printf("\n");
}

} // namespace Tokeniser