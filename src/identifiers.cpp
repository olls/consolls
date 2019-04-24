#include "identifiers.h"

#include "allocate.h"


namespace Compolls
{

namespace Identifiers
{

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


ID
add(Identifiers& identifiers, Identifier const& identifier)
{
  ID result = InvalidID;

  ID existing_name = find(identifiers, identifier.string);
  if (existing_name == InvalidID)
  {
    result = identifiers.n_elements;
    Array::add(identifiers, identifier);
  }

  assert(result != InvalidID);
  return result;
}


void
add16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "16_sig_16_16");
  type.function.return_type = types.built_in.u16_type;
  type.function.arg_types = Allocate::copy<TypeSystem::ID>(type.function.n_arg_types, {
    types.built_in.u16_type,
    types.built_in.u16_type
  });

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "add16");
  identifier.type = type_id;

  add(identifiers, identifier);
}


void
add8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "8_sig_8_8");
  type.function.return_type = types.built_in.u8_type;
  type.function.arg_types = Allocate::copy<TypeSystem::ID>(type.function.n_arg_types, {
    types.built_in.u8_type,
    types.built_in.u8_type
  });

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "add8");
  identifier.type = type_id;

  add(identifiers, identifier);
}


void
neg8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "8_sig_8");
  type.function.return_type = types.built_in.u8_type;
  type.function.arg_types = Allocate::copy<TypeSystem::ID>(type.function.n_arg_types, {
    types.built_in.u8_type,
  });

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "neg8");
  identifier.type = type_id;

  add(identifiers, identifier);
}


void
neg16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "16_sig_16");
  type.function.return_type = types.built_in.u16_type;
  type.function.arg_types = Allocate::copy<TypeSystem::ID>(type.function.n_arg_types, {
    types.built_in.u16_type
  });

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "neg16");
  identifier.type = type_id;

  add(identifiers, identifier);
}


void
init_built_in_identifiers(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  add8_built_in(identifiers, strings, types);
  add16_built_in(identifiers, strings, types);
  neg8_built_in(identifiers, strings, types);
  neg16_built_in(identifiers, strings, types);
}

} // namespace Identifiers

} // namespace Compolls