#include "tokeniser.h"


namespace Tokeniser
{

Array::Array<Token>
tokenise(String::String text)
{
  Array::Array<Token> result = {};

  bool in_token = false;
  u32 token_start;
  Token token = {};

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
        token.end = character_index;
        Array::add(result, token);
      }
    }

    if (!in_token)
    {
      if (!String::is_whitespace(c))
      {
        in_token = true;
        token.start = character_index;
      }
    }
  }

  if (in_token)
  {
    in_token = false;
    token.end = text.length;
    Array::add(result, token);
  }

  return result;
}


String::String
string(String::String const & text, Token const & token)
{
  String::String result = {
    .start = text.start + token.start,
    .length = token.end - token.start
  };
  return result;
}

} // namespace Tokeniser
