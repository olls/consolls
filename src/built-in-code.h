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
// TODO: Write an inliner at the ast stage


inline
FunctionInfo
generate_add8_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  FunctionInfo result = {};
  result.return_value_bytes += 1;
  result.data_start = addr;

  result.return_address = Machine::advance_addr<MemoryAddress>(addr);
  result.return_value = Machine::advance_addr<u8>(addr);

  // Params
  MemoryAddress param_a = Machine::advance_addr<u8>(addr);
  result.parameter_addrs += param_a;

  MemoryAddress param_b = Machine::advance_addr<u8>(addr);
  result.parameter_addrs += param_b;

  // Code
  result.data_end = addr;
  result.code_start = result.data_end;

  Basolls::push_instruction<Code::ADD>(*code_generator.machine, addr, {
    .a = param_a,
    .b = param_b,
    .result = result.return_value
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = result.return_address
  });

  result.code_end = addr;

  return result;
}


inline
FunctionInfo
generate_add16_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  FunctionInfo result = {};
  result.return_value_bytes += 2;
  result.data_start = addr;

  result.return_address = Machine::advance_addr<MemoryAddress>(addr);
  result.return_value = Machine::advance_addr<u16>(addr);

  // Params
  MemoryAddress param_a = Machine::advance_addr<u16>(addr);
  result.parameter_addrs += param_a;

  MemoryAddress param_b = Machine::advance_addr<u16>(addr);
  result.parameter_addrs += param_b;

  // Code
  result.data_end = addr;
  result.code_start = result.data_end;

  Basolls::push_instruction<Code::ADD_W>(*code_generator.machine, addr, {
    .a = param_a,
    .b = param_b,
    .result = result.return_value
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = result.return_address
  });

  result.code_end = addr;

  return result;
}


inline
FunctionInfo
generate_sub8_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  FunctionInfo result = {};
  result.return_value_bytes += 1;
  result.data_start = addr;

  result.return_address = Machine::advance_addr<MemoryAddress>(addr);
  result.return_value = Machine::advance_addr<u8>(addr);

  // Params
  MemoryAddress param_a = Machine::advance_addr<u8>(addr);
  result.parameter_addrs += param_a;

  MemoryAddress param_b = Machine::advance_addr<u8>(addr);
  result.parameter_addrs += param_b;

  // Code
  result.data_end = addr;
  result.code_start = result.data_end;

  Basolls::push_instruction<Code::SUB>(*code_generator.machine, addr, {
    .a = param_a,
    .b = param_b,
    .result = result.return_value
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = result.return_address
  });

  result.code_end = addr;

  return result;
}


inline
FunctionInfo
generate_sub16_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  FunctionInfo result = {};
  result.return_value_bytes += 2;
  result.data_start = addr;

  result.return_address = Machine::advance_addr<MemoryAddress>(addr);
  result.return_value = Machine::advance_addr<u16>(addr);

  // Params
  MemoryAddress param_a = Machine::advance_addr<u16>(addr);
  result.parameter_addrs += param_a;

  MemoryAddress param_b = Machine::advance_addr<u16>(addr);
  result.parameter_addrs += param_b;

  // Code
  result.data_end = addr;
  result.code_start = result.data_end;

  Basolls::push_instruction<Code::SUB_W>(*code_generator.machine, addr, {
    .a = param_a,
    .b = param_b,
    .result = result.return_value
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = result.return_address
  });

  result.code_end = addr;

  return result;
}


inline
FunctionInfo
generate_put8_code(CodeGenerator& code_generator, MemoryAddress& addr)
{
  FunctionInfo result = {};
  result.return_value_bytes += 2;
  result.data_start = addr;

  result.return_address = Machine::advance_addr<MemoryAddress>(addr);
  result.return_value = Machine::advance_addr<u8>(addr);

  // Params
  MemoryAddress address = Machine::advance_addr<MemoryAddress>(addr);
  result.parameter_addrs += address;

  MemoryAddress value = Machine::advance_addr<u8>(addr);
  result.parameter_addrs += value;

  // Code
  result.data_end = addr;
  result.code_start = result.data_end;

  Basolls::push_instruction<Code::SET>(*code_generator.machine, addr, {
    .from = value,
    .to_p = address
  });

  Basolls::push_instruction<Code::JUMP>(*code_generator.machine, addr, {
    .addr = result.return_address
  });

  result.code_end = addr;

  return result;
}


inline
void
generate_code(CodeGenerator& code_generator, Identifiers::BuiltIn built_in_identifiers, FunctionScope& func)
{
  {
    FunctionInfo func_info = generate_add8_code(code_generator, func.info.data_end);
    func.identifiers_map += {
      built_in_identifiers.add8,
      {
        .address = func_info.code_start,
        .debug_name = Strings::add(*code_generator.strings, "add8")
      }
    };
    func.functions_map += {
      built_in_identifiers.add8,
      func_info
    };
  }
  {
    FunctionInfo func_info = generate_add16_code(code_generator, func.info.data_end);
    func.identifiers_map += {
      built_in_identifiers.add16,
      {
        .address = func_info.code_start,
        .debug_name = Strings::add(*code_generator.strings, "add16")
      }
    };
    func.functions_map += {
      built_in_identifiers.add16,
      func_info
    };
  }
  {
    FunctionInfo func_info = generate_sub8_code(code_generator, func.info.data_end);
    func.identifiers_map += {
      built_in_identifiers.sub8,
      {
        .address = func_info.code_start,
        .debug_name = Strings::add(*code_generator.strings, "sub8")
      }
    };
    func.functions_map += {
      built_in_identifiers.sub8,
      func_info
    };
  }
  {
    FunctionInfo func_info = generate_sub16_code(code_generator, func.info.data_end);
    func.identifiers_map += {
      built_in_identifiers.sub16,
      {
        .address = func_info.code_start,
        .debug_name = Strings::add(*code_generator.strings, "sub16")
      }
    };
    func.functions_map += {
      built_in_identifiers.sub16,
      func_info
    };
  }
  {
    FunctionInfo func_info = generate_put8_code(code_generator, func.info.data_end);
    func.identifiers_map += {
      built_in_identifiers.put8,
      {
        .address = func_info.code_start,
        .debug_name = Strings::add(*code_generator.strings, "put8")
      }
    };
    func.functions_map += {
      built_in_identifiers.put8,
      func_info
    };
  }
}

} // namespace BuiltInCode

} // namespace CodeGenerator

} // namespace Compolls