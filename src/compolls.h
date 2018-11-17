#pragma once

#include "basolls.h"
#include "machine.h"
#include "instructions.h"
#include "string.h"
#include "types.h"


namespace Compolls
{

using Instructions::Code;
using Instructions::Args;
using Machine::MemoryAddress;


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


Basolls::Subroutine<void>
compile(String::String text);

} // namespace Compolls