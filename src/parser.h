#pragma once

#include "parse-tree.h"
#include "tokeniser.h"
#include "string.h"
#include "stack.h"
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


struct Parser
{
  String::String text;
  Stack::Stack<Tokeniser::Token> tokens;

  SymbolLookahead<2> lookahead;

  u32 depth;
};


Symbol
advance_terminal(Parser& parser);


bool
program(Parser& parser, Tree::Node** result);

} // namespace Parser

} // namespace Compolls
