#pragma once

#include "parse-tree.h"
#include "tokeniser.h"
#include "strings.h"
#include "utils/string.h"
#include "utils/fifo.h"
#include "utils/types.h"


namespace Compolls
{

namespace Parser
{

enum class SymbolType : u8
{
  Error,
  L_Parenthesis,  // (
  R_Parenthesis,  // )
  L_Brace,  // {
  R_Brace,  // }
  L_Bracket,  // [
  R_Bracket,  // ]
  Equals,
  Comma,
  Identifier,
  Func,
  Number
};


struct Symbol
{
  SymbolType type;
  Tokeniser::Token token;
};


template <u32 t_lookahead_n>
struct SymbolLookahead
{
  static u32 const lookahead_n = t_lookahead_n;
  Symbol symbols[lookahead_n];
};


template <u32 n>
inline bool
lookahead_empty(SymbolLookahead<n> const & lookahead)
{
  bool result = true;
  for (u32 i = 0;
       i < n;
       ++i)
  {
    result &= lookahead.symbols[i].type == SymbolType::Error;
  }
  return result;
}


template <u32 n>
inline void
print_lookahead(Strings::Table const & strings, SymbolLookahead<n> const & lookahead)
{
  for (u32 i = 0;
       i < n;
       ++i)
  {
    if (lookahead.symbols[i].type == SymbolType::Error)
    {
      break;
    }
    else
    {
      if (i != 0)
      {
        printf(", ");
      }
      printf("\"%.*s\"", print_s(Tokeniser::string(strings, lookahead.symbols[i].token)));
    }
  }
}


struct Parser
{
  String::String text;
  Fifo::Fifo<Tokeniser::Token> tokens;
  Strings::Table* strings;

  using SymbolLookaheadType = SymbolLookahead<4>;
  SymbolLookaheadType lookahead;

  u32 last_symbol_end_position;

  u32 depth;
};


Symbol
advance_terminal(Parser& parser);


bool
program(Parser& parser, Tree::Node** result);


inline
void
prime_lookahead(Parser& parser)
{
  for (u32 index = 0;
       index < parser.lookahead.lookahead_n;
       ++index)
  {
    parser.lookahead.symbols[index] = advance_terminal(parser);
  }
}


inline
bool
validate_end_state(Parser const& parser)
{
  bool success = true;

  if (!lookahead_empty(parser.lookahead))
  {
    success &= false;
    printf("Junk tokens at end of program: ");
    print_lookahead(*parser.strings, parser.lookahead);
    printf("\n");
  }
  else if (!Fifo::empty(parser.tokens))
  {
    success &= false;
    printf("Lookahead empty but junk tokens remaining in token queue: ");
    Tokeniser::print(*parser.strings, parser.tokens.elements, parser.tokens.position);
  }

  return success;
}

} // namespace Parser

} // namespace Compolls
