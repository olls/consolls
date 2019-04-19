#include "identifiers.h"

#include "allocate.h"


namespace Compolls
{

namespace Identifiers
{

void
add16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types, TypeSystem::BuiltInTypes const & built_in_types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = built_in_types.func_string;
  type.function.return_type = built_in_types.u16_type;
  type.function.n_arg_types = 2;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  type.function.arg_types[0] = built_in_types.u16_type;
  type.function.arg_types[1] = built_in_types.u16_type;

  TypeSystem::ID type_id = types.n_elements;
  TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "add16");
  identifier.type = type_id;

  Array::add(identifiers, identifier);
}


void
add8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types, TypeSystem::BuiltInTypes const & built_in_types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = built_in_types.func_string;
  type.function.return_type = built_in_types.u8_type;
  type.function.n_arg_types = 2;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  type.function.arg_types[0] = built_in_types.u8_type;
  type.function.arg_types[1] = built_in_types.u8_type;

  TypeSystem::ID type_id = types.n_elements;
  TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "add8");
  identifier.type = type_id;

  Array::add(identifiers, identifier);
}


void
neg8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types, TypeSystem::BuiltInTypes const & built_in_types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = built_in_types.func_string;
  type.function.return_type = built_in_types.u8_type;
  type.function.n_arg_types = 1;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  type.function.arg_types[0] = built_in_types.u8_type;

  TypeSystem::ID type_id = types.n_elements;
  TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "neg8");
  identifier.type = type_id;

  Array::add(identifiers, identifier);
}


void
neg16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types, TypeSystem::BuiltInTypes const & built_in_types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = built_in_types.func_string;
  type.function.return_type = built_in_types.u16_type;
  type.function.n_arg_types = 1;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  type.function.arg_types[0] = built_in_types.u16_type;

  TypeSystem::ID type_id = types.n_elements;
  TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "neg16");
  identifier.type = type_id;

  Array::add(identifiers, identifier);
}


void
init_built_in_identifiers(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types, TypeSystem::BuiltInTypes const & built_in_types)
{
  add8_built_in(identifiers, strings, types, built_in_types);
  add16_built_in(identifiers, strings, types, built_in_types);
  neg8_built_in(identifiers, strings, types, built_in_types);
  neg16_built_in(identifiers, strings, types, built_in_types);
}


ID
find(Identifiers const & identifiers, Strings::ID string)
{
  ID result = InvalidID;

  for (u32 index = 0;
       index < identifiers.n_elements;
       ++index)
  {
    Identifier const & identifier = identifiers[index];
    if (identifier.string == string)
    {
      result = index;
      break;
    }
  }
  return result;
}

} // namespace Identifiers

} // namespace Compolls