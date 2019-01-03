#include "type-system.h"


namespace Compolls
{

namespace TypeSystem
{

void
init_built_in_symbols(Types& types, BuiltInTypes& built_in_types, Symbols::Table& symbols)
{
  built_in_types.u8_symbol = Symbols::add(symbols, S("u8"));
  built_in_types.u16_symbol = Symbols::add(symbols, S("u16"));
  built_in_types.func_symbol = Symbols::add(symbols, S("Func"));

  built_in_types.u8_type = add(types, built_in_types, built_in_types.u8_symbol);
  built_in_types.u16_type = add(types, built_in_types, built_in_types.u16_symbol);
  built_in_types.func_type = add(types, built_in_types, built_in_types.func_symbol);
}


ID
add(Types& types, Type const & type)
{
  ID result = types.n_elements;
  Array::add(types, type);

  printf("Type(%u), symbol(%u)", result, type.symbol);
  printf(", type: ");
  switch (type.type)
  {
    case (Type::BuiltIn::U8):
    {
      printf("U8");
    } break;
    case (Type::BuiltIn::U16):
    {
      printf("U16");
    } break;
    case (Type::BuiltIn::Func):
    {
      printf("Func, return_type: %u, n_args: %u", type.function.return_type, type.function.n_arg_types);
    } break;
  }
  printf("\n");
  return result;
}


ID
add(Types& types, BuiltInTypes const & built_in_types, Symbols::ID type_symbol)
{
  ID result;

  Type type = { .symbol = type_symbol };

  if (type_symbol == built_in_types.u8_symbol)
  {
    type.type = Type::BuiltIn::U8;
  }
  else if (type_symbol == built_in_types.u16_symbol)
  {
    type.type = Type::BuiltIn::U16;
  }
  else if (type_symbol == built_in_types.func_symbol)
  {
    type.type = Type::BuiltIn::Func;
  }
  else
  {
    assert(0);
  }

  result = add(types, type);

  return result;
}


ID
find(Types const & types, Symbols::ID type_symbol)
{
  ID result = TypeSystem::InvalidID;

  Array::FindFirstFunc<Type> find_func = [](Type const & t, void* user){ return t.symbol == *(Symbols::ID*)user; };
  Type const* type = Array::find_first(types, find_func, &type_symbol);

  if (type != NULL)
  {
    result = type->symbol;
  }

  return result;
}


bool
resolve_literal_symbol_type(Types const & types, BuiltInTypes const & built_in_types, ID type, Parser::Tree::LiteralNode::Type *result)
{
  bool success = true;

  using Parser::Tree::LiteralNode;

  if (type == built_in_types.u8_type ||
      type == built_in_types.u16_type)
  {
    *result = LiteralNode::Type::Number;
  }
  else if (type == built_in_types.func_type)
  {
    *result = LiteralNode::Type::Function;
  }
  else
  {
    printf("Not a built in type ID: %u\n", type);
    success &= false;
  }

  return success;
}


StringArray::StringArray
string(Type const & type)
{
  StringArray::StringArray result = {};

  switch (type.type)
  {
    case (Type::BuiltIn::U8):
    {
      result += S("u8");
    } break;
    case (Type::BuiltIn::U16):
    {
      result += S("u16");
    } break;
    case (Type::BuiltIn::Func):
    {
      result += S("Func");
    } break;
  }

  result += String::string_f("(%u)", type.symbol);

  return result;
}

} // namespace TypeSystem

} // namespace Compolls
