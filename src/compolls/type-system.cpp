#include "type-system.h"

#include "utils/debug.h"


namespace Compolls
{

namespace TypeSystem
{

void
init_built_in_types(Types& types)
{
  types.built_in.u8_string = Strings::add(*types.strings, "u8");
  types.built_in.u16_string = Strings::add(*types.strings, "u16");

  types.built_in.u8_type = add(types, (Type){.type = Type::BuiltIn::U8, .string = types.built_in.u8_string});
  types.built_in.u16_type = add(types, (Type){.type = Type::BuiltIn::U16, .string = types.built_in.u16_string});
}


ID
find(Types const& types, Strings::ID type_string)
{
  ID result = InvalidID;

  Array::FindFirstFunc<Type> find_func = [](Type const& t, void* user){ return t.string == *(Strings::ID*)user; };
  s32 index = Array::find_first(types.types, find_func, &type_string);
  if (index != -1)
  {
    result = (ID)index;
  }
  return result;
}


ID
find(Types const& types, Type const& type)
{
  ID result = InvalidID;

  s32 index = Array::find_first(types.types, type);
  if (index != -1)
  {
    result = (ID)index;
  }
  return result;
}


ID
add(Types& types, Type const& type)
{
  ID result = InvalidID;

  ID exists = find(types, type);
  if (exists != InvalidID)
  {
    // TODO: Proper error handling here?
    result = exists;
  }
  else
  {
    result = types.types.n_elements;
    Array::add(types.types, type);
  }

  printf("Adding ");
  StringArray::StringArray type_string = {};
  string(types, type, type_string);
  StringArray::print(type_string);
  Array::free_array(type_string);
  printf(" <- ");
  if (result == exists)
  {
    printf("Type already exists: ");
  }
  printf("%u\n", result);

  return result;
}


Parser::Tree::LiteralNode::Type
get_symbol_type(Types const& types, ID type_id)
{
  using Parser::Tree::LiteralNode;
  LiteralNode::Type result;

  Type const& type = types.types[type_id];

  switch (type.type)
  {
    case (Type::BuiltIn::U8):
    case (Type::BuiltIn::U16):
    {
      result = LiteralNode::Type::Number;
    } break;
    case (Type::BuiltIn::Func):
    {
      result = LiteralNode::Type::Function;
    } break;
  }

  return result;
}


void
string(Types const& types, Type const& type, StringArray::StringArray& result)
{
  result += "{\"";
  result += Strings::get(*types.strings, type.string);
  result += "\" ";

  result += "type: ";
  switch (type.type)
  {
    case (Type::BuiltIn::U8):
    {
      result += "U8";
    } break;
    case (Type::BuiltIn::U16):
    {
      result += "U16";
    } break;
    case (Type::BuiltIn::Func):
    {
      result += "Func, return_type: ";
      string(types, types.types[type.function.return_type], result);
      result += String::format(", n_args: %u", type.function.n_arg_types);
      for (u32 i = 0;
           i < type.function.n_arg_types;
           ++i)
      {
        ID arg_type = type.function.arg_types[i];
        result += String::format(", arg%u: ", i);
        string(types, types.types[arg_type], result);
      }
    } break;
  }
  result += "}";
}

} // namespace TypeSystem

} // namespace Compolls
