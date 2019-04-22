#include "tokeniser.h"


namespace Tokeniser
{

void
add_token(String::String text, Array::Array<Token>& tokens, Strings::Table& strings, u32 start, u32 end)
{
  Token token = {
    .start = start,
    .end = end,
    .string = Strings::add(strings, String::String(text.start + start, end - start))
  };

  Array::add(tokens, token);
}


Array::Array<Token>
tokenise(String::String text, Strings::Table& strings)
{
  Array::Array<Token> result = {};

  bool in_token = false;
  u32 token_start;

  bool in_line_comment = false;
  bool in_inline_comment = false;

  char c = '\0';

  for (u32 character_index = 0;
       character_index < text.length;
       ++character_index)
  {
    char prev_c = c;
    c = text.start[character_index];

    char next_c = '\0';
    if (character_index + 1 < text.length)
    {
      next_c = text.start[character_index + 1];
    }

    bool skip_c = false;

    if (!in_line_comment && !in_inline_comment)
    {
      if (c == '/' && next_c == '*')
      {
        in_inline_comment = true;
        skip_c = true;
      }
    }

    if (!in_line_comment && !in_inline_comment)
    {
      if (c == '/' && next_c == '/')
      {
        in_line_comment = true;
        skip_c = true;
      }
    }

    if ((in_inline_comment || in_line_comment) && in_token)
    {
      in_token = false;
      add_token(text, result, strings, token_start, character_index);
    }

    if (in_inline_comment)
    {
      if (c == '*' &&
          next_c == '/')
      {
        in_inline_comment = false;
        skip_c = true;
      }
    }
    else if (in_line_comment)
    {
      if (c == '\n')
      {
        in_line_comment = false;
      }
    }

    if (skip_c)
    {
      character_index += 1;
    }
    else if (!in_inline_comment && !in_line_comment)
    {
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
  }

  if (in_token)
  {
    in_token = false;
    add_token(text, result, strings, token_start, text.length);
  }

  return result;
}

} // namespace Tokeniser
