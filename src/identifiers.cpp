#include "identifiers.h"

#include "allocate.h"


namespace Compolls
{

namespace Identifiers
{

void
add16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "16_sig_16_16");
  type.function.return_type = types.built_in.u16_type;
  type.function.n_arg_types = 2;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  type.function.arg_types[0] = types.built_in.u16_type;
  type.function.arg_types[1] = types.built_in.u16_type;

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "add16");
  identifier.type = type_id;

  Array::add(identifiers, identifier);
}


void
add8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "8_sig_8_8");
  type.function.return_type = types.built_in.u8_type;
  type.function.n_arg_types = 2;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  type.function.arg_types[0] = types.built_in.u8_type;
  type.function.arg_types[1] = types.built_in.u8_type;

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "add8");
  identifier.type = type_id;

  Array::add(identifiers, identifier);
}


void
neg8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "8_sig_8");
  type.function.return_type = types.built_in.u8_type;
  type.function.n_arg_types = 1;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  type.function.arg_types[0] = types.built_in.u8_type;

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "neg8");
  identifier.type = type_id;

  Array::add(identifiers, identifier);
}


void
neg16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "16_sig_16");
  type.function.return_type = types.built_in.u16_type;
  type.function.n_arg_types = 1;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  type.function.arg_types[0] = types.built_in.u16_type;

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "neg16");
  identifier.type = type_id;

  Array::add(identifiers, identifier);
}


void
init_built_in_identifiers(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  add8_built_in(identifiers, strings, types);
  add16_built_in(identifiers, strings, types);
  neg8_built_in(identifiers, strings, types);
  neg16_built_in(identifiers, strings, types);
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