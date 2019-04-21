#pragma once

#include "strings.h"
#include "string.h"
#include "array.h"
#include "types.h"


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


// Depricated
inline
String::String
string(String::String const & text, Token const & token)
{
  return String::sub_string(text, token.start, token.end);
}


inline
String::String
string(Strings::Table const & strings, Token const & token)
{
  return Strings::get(strings, token.string);
}


inline
void
print(Strings::Table const & strings, Array::Array<Token> const & tokens, u32 start_pos = 0)
{
  for (u32 token_index = start_pos;
       token_index < tokens.n_elements;
       ++token_index)
  {
    printf("\"%.*s\", ", print_s(string(strings, tokens[token_index])));
  }
  printf("\n");
}

} // namespace Tokeniser