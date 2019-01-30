#pragma once

#include "parse-tree.h"
#include "tokeniser.h"
#include "string.h"
#include "fifo.h"
#include "types.h"


namespace Compolls
{

namespace Parser
{

enum class SymbolType : u8
{
  Error,
  Whitespace,
  L_Parenthesis,  // (
  R_Parenthesis,  // )
  L_Brace,  // {
  R_Brace,  // }
  L_Bracket,  // [
  R_Bracket,  // ]
  Equals,
  Comma,
  Identifier,
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
  bool result = false;
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
print_lookahead(String::String const & text, SymbolLookahead<n> const & lookahead)
{
  for (u32 i = 0;
       i < n;
       ++i)
  {
    if (lookahead.symbols[i].type != SymbolType::Error)
    {
      printf("\"%.*s\", ", print_s(Tokeniser::string(text, lookahead.symbols[i].token)));
    }
  }
  printf("\n\n");
}


struct Parser
{
  String::String text;
  Fifo::Fifo<Tokeniser::Token> tokens;

  using SymbolLookaheadType = SymbolLookahead<2>;
  SymbolLookaheadType lookahead;

  u32 depth;
};


Symbol
advance_terminal(Parser& parser);


bool
program(Parser& parser, Tree::Node** result);

} // namespace Parser

} // namespace Compolls
