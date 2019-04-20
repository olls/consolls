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


String::String
string(String::String const & text, Token const & token);


inline
void
print(String::String const & text, Array::Array<Token> const & tokens, u32 start_pos = 0)
{
  for (u32 token_index = start_pos;
       token_index < tokens.n_elements;
       ++token_index)
  {
    printf("\"%.*s\", ", print_s(string(text, tokens[token_index])));
  }
  printf("\n");
}

} // namespace Tokeniser