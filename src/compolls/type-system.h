#pragma once

#include "parse-tree.h"
#include "strings.h"

#include "utils/string-array.h"
#include "utils/string.h"
#include "utils/array.h"
#include "utils/types.h"


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

  // TODO: Do we really want to store a string with the type?  It definitely isn't needed for function types?
  Strings::ID string;
};


struct Types
{
  Array::Array<Type> types;

  // Store the ids of the built-in-types here to save always looking them up
  struct BuiltInTypes
  {
    Strings::ID u8_string;
    ID u8_type;
    Strings::ID u16_string;
    ID u16_type;
  } built_in;

  Strings::Table* strings;
};


inline
bool
operator==(Type const& a, Type const& b)
{
  bool result = (a.type == b.type);

  if (result)
  {
    switch (a.type)
    {
      case (Type::BuiltIn::U8):
      case (Type::BuiltIn::U16):
      {
        // TODO: we are doing a string comparison of the u8/u16 types here,
        //       but this isn't necessarily what we want.  i.e: do we want
        //       to allow implicit casting of types which share the
        //       underlying base types?
        result &= (a.string == b.string);

      } break;
      case (Type::BuiltIn::Func):
      {
        result &= a.function.return_type == b.function.return_type;
        result &= a.function.n_arg_types == b.function.n_arg_types;

        for (u32 i = 0;
             i < a.function.n_arg_types;
             ++i)
        {
          result &= a.function.arg_types[i] == b.function.arg_types[i];
        }
      } break;
    }
  }

  return result;
}


void
init_built_in_types(Types& types);


ID
add(Types& types, Type const& type);


ID
find(Types const& types, Strings::ID type_string);


Parser::Tree::LiteralNode::Type
get_symbol_type(Types const& types, ID type_id);


void
string(Types const& types, Type const& type, StringArray::StringArray& result);


// string wrapper for calling with a type id
inline
void
string(Types const& types, ID const type_id, StringArray::StringArray& result)
{
  string(types, types.types[type_id], result);
}


// string wrapper for concatenating the StringArray to a String
inline
String::String
string(Types const& types, ID type)
{
  StringArray::StringArray arr = {};
  string(types, type, arr);
  String::String const& result = StringArray::concat(arr);
  Array::free_array(arr);
  return result;
}


// Create a StringArray listing all types
inline
void
string(Types const& types, StringArray::StringArray& result)
{
  for (ID index = 0;
       index < types.types.n_elements;
       ++index)
  {
    Type const& type = types.types[index];
    result += String::format("%u: ", index);
    string(types, type, result);
    if (index != types.types.n_elements-1)
    {
      result += ",\n";
    }
  }
}

} // namespace TypeSystem

} // namespace Compolls
