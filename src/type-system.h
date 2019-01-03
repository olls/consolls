#pragma once

#include "parse-tree.h"
#include "collection-stack.h"
#include "symbols.h"
#include "string-array.h"
#include "types.h"


namespace Compolls
{

namespace TypeSystem
{

using ID = u32;

static ID const InvalidID = (u32)-1;


struct Type
{
  enum class BuiltIn
  {
    U8,
    U16,
    Func
  } type;

  union
  {
    struct
    {
      ID return_type;

      u32 n_arg_types;
      ID* arg_types;
    } function;
  };

  Symbols::ID symbol;
};


using Types = Array::Array<Type>;


struct BuiltInTypes
{
  Symbols::ID u8_symbol;
  ID u8_type;
  Symbols::ID u16_symbol;
  ID u16_type;
  Symbols::ID func_symbol;
  ID func_type;
};


void
init_built_in_symbols(Types& types, BuiltInTypes& built_in_types, Symbols::Table& symbols);


ID
add(Types& types, Type const & type);


ID
add(Types& types, BuiltInTypes const & built_in_types, Symbols::ID type_symbol);


ID
find(Types const & types, Symbols::ID type_symbol);


bool
resolve_literal_symbol_type(Types const & types, BuiltInTypes const & built_in_types, ID type, Parser::Tree::LiteralNode::Type *result);


StringArray::StringArray
string(Type const & type);

} // namespace TypeSystem

} // namespace Compolls