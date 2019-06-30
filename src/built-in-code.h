#pragma once

#include "code-generator.h"
#include "ast.h"
#include "basolls.h"
#include "identifiers.h"
#include "machine.h"
#include "strings.h"
#include "string.h"
#include "types.h"


namespace Compolls
{

namespace CodeGenerator
{

namespace BuiltInCode
{

using Machine::MemoryAddress;


// TODO: These should probably be inlined


inline
MemoryAddress
generate_add8_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  MemoryAddress return_addr = Machine::advance_addr<MemoryAddress>(addr);
  MemoryAddress return_value = Machine::advance_addr<u8>(addr);

  // Params
  MemoryAddress param_a = Machine::advance_addr<u8>(addr);
  MemoryAddress param_b = Machine::advance_addr<u8>(addr);

  // Code
  MemoryAddress result = addr;

  Basolls::push_instruction<Code::ADD>(*code_generator.machine, addr, {
    .a = param_a,
    .b = param_b,
    .result = return_value
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = return_addr
  });

  return result;
}


inline
MemoryAddress
generate_add16_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  MemoryAddress return_addr = Machine::advance_addr<MemoryAddress>(addr);
  MemoryAddress return_value = Machine::advance_addr<u16>(addr);

  // Params
  MemoryAddress param_a = Machine::advance_addr<u16>(addr);
  MemoryAddress param_b = Machine::advance_addr<u16>(addr);

  // Code
  MemoryAddress result = addr;

  Basolls::push_instruction<Code::ADD_W>(*code_generator.machine, addr, {
    .a = param_a,
    .b = param_b,
    .result = return_value
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = return_addr
  });

  return result;
}


inline
MemoryAddress
generate_sub8_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  MemoryAddress return_addr = Machine::advance_addr<MemoryAddress>(addr);
  MemoryAddress return_value = Machine::advance_addr<u8>(addr);

  // Params
  MemoryAddress param_a = Machine::advance_addr<u8>(addr);
  MemoryAddress param_b = Machine::advance_addr<u8>(addr);

  // Code
  MemoryAddress result = addr;

  Basolls::push_instruction<Code::SUB>(*code_generator.machine, addr, {
    .a = param_a,
    .b = param_b,
    .result = return_value
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = return_addr
  });

  return result;
}


inline
MemoryAddress
generate_sub16_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  MemoryAddress return_addr = Machine::advance_addr<MemoryAddress>(addr);
  MemoryAddress return_value = Machine::advance_addr<u16>(addr);

  // Params
  MemoryAddress param_a = Machine::advance_addr<u16>(addr);
  MemoryAddress param_b = Machine::advance_addr<u16>(addr);

  // Code
  MemoryAddress result = addr;

  Basolls::push_instruction<Code::SUB_W>(*code_generator.machine, addr, {
    .a = param_a,
    .b = param_b,
    .result = return_value
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = return_addr
  });

  return result;
}


inline
MemoryAddress
generate_put8_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  MemoryAddress return_addr = Machine::advance_addr<MemoryAddress>(addr);
  MemoryAddress return_value = Machine::advance_addr<u8>(addr);

  // Params
  MemoryAddress address = Machine::advance_addr<MemoryAddress>(addr);
  MemoryAddress value = Machine::advance_addr<u8>(addr);

  // Code
  MemoryAddress result = addr;

  Basolls::push_instruction<Code::SET>(*code_generator.machine, addr, {
    .from = value,
    .to_p = address
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = return_addr
  });

  return result;
}


inline
void
generate_code(CodeGenerator& code_generator, Identifiers::BuiltIn built_in_identifiers, FunctionScope& func)
{
  func.identifiers_map += {
    built_in_identifiers.add8,
    {
      .address = generate_add8_code(code_generator, func.info.data_end),
      .debug_name = Strings::add(*code_generator.strings, "add8")
    }
  };
  func.identifiers_map += {
    built_in_identifiers.add16,
    {
      .address = generate_add16_code(code_generator, func.info.data_end),
      .debug_name = Strings::add(*code_generator.strings, "add16")
    }
  };
  func.identifiers_map += {
    built_in_identifiers.sub8,
    {
      .address = generate_sub8_code(code_generator, func.info.data_end),
      .debug_name = Strings::add(*code_generator.strings, "sub8")
    }
  };
  func.identifiers_map += {
    built_in_identifiers.sub16,
    {
      .address = generate_sub16_code(code_generator, func.info.data_end),
      .debug_name = Strings::add(*code_generator.strings, "sub16")
    }
  };
  func.identifiers_map += {
    built_in_identifiers.put8,
    {
      .address = generate_put8_code(code_generator, func.info.data_end),
      .debug_name = Strings::add(*code_generator.strings, "put8")
    }
  };
}

} // namespace BuiltInCode

} // namespace CodeGenerator

} // namespace Compolls