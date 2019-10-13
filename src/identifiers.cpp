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


ID
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

  return add(identifiers, identifier);
}


ID
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

  return add(identifiers, identifier);
}


ID
sub8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
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
  identifier.string = Strings::add(strings, "sub8");
  identifier.type = type_id;

  return add(identifiers, identifier);
}


ID
sub16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
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
  identifier.string = Strings::add(strings, "sub16");
  identifier.type = type_id;

  return add(identifiers, identifier);
}


ID
mul8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
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
  identifier.string = Strings::add(strings, "mul8");
  identifier.type = type_id;

  return add(identifiers, identifier);
}


ID
mul16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
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
  identifier.string = Strings::add(strings, "mul16");
  identifier.type = type_id;

  return add(identifiers, identifier);
}


ID
div8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
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
  identifier.string = Strings::add(strings, "div8");
  identifier.type = type_id;

  return add(identifiers, identifier);
}


ID
div16_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
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
  identifier.string = Strings::add(strings, "div16");
  identifier.type = type_id;

  return add(identifiers, identifier);
}


ID
put8_built_in(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  TypeSystem::Type type = {};
  type.type = TypeSystem::Type::BuiltIn::Func;
  type.string = Strings::add(strings, "8_sig_16_8");
  type.function.return_type = types.built_in.u8_type;
  type.function.arg_types = Allocate::copy<TypeSystem::ID>(type.function.n_arg_types, {
    types.built_in.u16_type,
    types.built_in.u8_type
  });

  TypeSystem::ID type_id = TypeSystem::add(types, type);

  Identifier identifier = {};
  identifier.string = Strings::add(strings, "put8");
  identifier.type = type_id;

  return add(identifiers, identifier);
}


BuiltIn
init_built_in_identifiers(Identifiers& identifiers, Strings::Table& strings, TypeSystem::Types& types)
{
  BuiltIn result = {};
  result.add8 = add8_built_in(identifiers, strings, types);
  result.add16 = add16_built_in(identifiers, strings, types);
  result.sub8 = sub8_built_in(identifiers, strings, types);
  result.sub16 = sub16_built_in(identifiers, strings, types);
  result.mul8 = mul8_built_in(identifiers, strings, types);
  result.mul16 = mul16_built_in(identifiers, strings, types);
  result.div8 = div8_built_in(identifiers, strings, types);
  result.div16 = div16_built_in(identifiers, strings, types);
  result.put8 = put8_built_in(identifiers, strings, types);
  return result;
}

} // namespace Identifiers

} // namespace Compolls