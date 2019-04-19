#include "type-system.h"


namespace Compolls
{

namespace TypeSystem
{

void
init_built_in_strings(Types& types, BuiltInTypes& built_in_types, Strings::Table& strings)
{
  built_in_types.u8_string = Strings::add(strings, "u8");
  built_in_types.u16_string = Strings::add(strings, "u16");
  built_in_types.func_string = Strings::add(strings, "Func");

  built_in_types.u8_type = add(types, built_in_types, built_in_types.u8_string);
  built_in_types.u16_type = add(types, built_in_types, built_in_types.u16_string);
  built_in_types.func_type = add(types, built_in_types, built_in_types.func_string);
}


ID
add(Types& types, Type const & type)
{
  ID result = types.n_elements;
  Array::add(types, type);

  printf("Type(%u), string(%u)", result, type.string);
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
add(Types& types, BuiltInTypes const & built_in_types, Strings::ID type_string)
{
  ID result;

  Type type = { .string = type_string };

  if (type_string == built_in_types.u8_string)
  {
    type.type = Type::BuiltIn::U8;
  }
  else if (type_string == built_in_types.u16_string)
  {
    type.type = Type::BuiltIn::U16;
  }
  else if (type_string == built_in_types.func_string)
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
find(Types const & types, Strings::ID type_string)
{
  ID result = TypeSystem::InvalidID;

  Array::FindFirstFunc<Type> find_func = [](Type const & t, void* user){ return t.string == *(Strings::ID*)user; };
  Type const* type = Array::find_first(types, find_func, &type_string);

  if (type != NULL)
  {
    result = type->string;
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
      result += "u8";
    } break;
    case (Type::BuiltIn::U16):
    {
      result += "u16";
    } break;
    case (Type::BuiltIn::Func):
    {
      result += "Func";
    } break;
  }

  result += String::string_f("(%u)", type.string);

  return result;
}

} // namespace TypeSystem

} // namespace Compolls
