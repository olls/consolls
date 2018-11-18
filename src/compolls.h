#pragma once

#include "basolls.h"
#include "machine.h"
#include "instructions.h"
#include "tokeniser.h"
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


struct Parser
{
  String::String text;
  Stack::Stack<Tokeniser::Token> tokens;

  Symbol symbol_a;
  Symbol symbol_b;

  u32 depth;
};

} // namespace Parser


Basolls::Subroutine<void>
compile(String::String text);

} // namespace Compolls