#pragma once

#include "parse-tree.h"
#include "strings.h"
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

  Strings::ID string;
};


using Types = Array::Array<Type>;


struct BuiltInTypes
{
  Strings::ID u8_string;
  ID u8_type;
  Strings::ID u16_string;
  ID u16_type;
  Strings::ID func_string;
  ID func_type;
};


void
init_built_in_strings(Types& types, BuiltInTypes& built_in_types, Strings::Table& strings);


ID
add(Types& types, Type const & type);


ID
add(Types& types, BuiltInTypes const & built_in_types, Strings::ID type_string);


ID
find(Types const & types, Strings::ID type_string);


bool
resolve_literal_symbol_type(Types const & types, BuiltInTypes const & built_in_types, ID type, Parser::Tree::LiteralNode::Type *result);


StringArray::StringArray
string(Type const & type);

} // namespace TypeSystem

} // namespace Compolls