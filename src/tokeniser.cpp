#include "tokeniser.h"


namespace Tokeniser
{

void
add_token(String::String text, Array::Array<Token>& tokens, Strings::Table& strings, u32 start, u32 end)
{
  Token token = {
    .start = start,
    .end = end,
    .symbol = Strings::add(strings, String::String(text.start + start, end - start))
  };

  Array::add(tokens, token);
}


Array::Array<Token>
tokenise(String::String text, Strings::Table& strings)
{
  Array::Array<Token> result = {};

  bool in_token = false;
  u32 token_start;

  char c = '\0';

  for (u32 character_index = 0;
       character_index < text.length;
       ++character_index)
  {
    char prev_c = c;
    c = text.start[character_index];

    if (in_token)
    {
      if (String::is_whitespace(c) ||
          !String::is_alpha_num(prev_c) ||
          !String::is_alpha_num(c))
      {
        in_token = false;
        add_token(text, result, strings, token_start, character_index);
      }
    }

    if (!in_token)
    {
      if (!String::is_whitespace(c))
      {
        in_token = true;
        token_start = character_index;
      }
    }
  }

  if (in_token)
  {
    in_token = false;
    add_token(text, result, strings, token_start, text.length);
  }

  return result;
}


String::String
string(String::String const & text, Token const & token)
{
  String::String result = {
    text.start + token.start,
    token.end - token.start
  };
  return result;
}

} // namespace Tokeniser
