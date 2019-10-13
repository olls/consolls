#include "disassembler.h"

#include "utils/debug.h"
#include "instructions.h"


namespace Disassembler
{

template <Instructions::Code Code>
void
disassemble_args(Machine::Machine const & machine, Instructions::Args<Code> args);

template <>
void
disassemble_args<Instructions::Code::ADD>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::ADD> args)
{
  printf("ADD a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::ADD_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::ADD_W> args)
{
  printf("ADD_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::SUB>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::SUB> args)
{
  printf("SUB a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::SUB_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::SUB_W> args)
{
  printf("SUB_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::MUL>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::MUL> args)
{
  printf("MUL a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::MUL_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::MUL_W> args)
{
  printf("MUL_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::DIV>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::DIV> args)
{
  printf("DIV a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::DIV_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::DIV_W> args)
{
  printf("DIV_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::NOT>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::NOT> args)
{
  printf("NOT in=%#.4x, result=%#.4x", args.in, args.result);
}

template <>
void
disassemble_args<Instructions::Code::LSHIFT>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::LSHIFT> args)
{
  printf("LSHIFT in=%#.4x, bits=%#.4x, result=%#.4x", args.in, args.bits, args.result);
}

template <>
void
disassemble_args<Instructions::Code::RSHIFT>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::RSHIFT> args)
{
  printf("RSHIFT in=%#.4x, bits=%#.4x, result=%#.4x", args.in, args.bits, args.result);
}

template <>
void
disassemble_args<Instructions::Code::AND>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::AND> args)
{
  printf("AND a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::OR>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::OR> args)
{
  printf("OR a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::XOR>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::XOR> args)
{
  printf("XOR a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::JUMP_V>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::JUMP_V> args)
{
  printf("JUMP_V %#.4x", args.addr);
}

template <>
void
disassemble_args<Instructions::Code::JUMP>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::JUMP> args)
{
  printf("JUMP %#.4x", args.addr);
}

template <>
void
disassemble_args<Instructions::Code::CJUMP>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::CJUMP> args)
{
  printf("CJUMP a=%#.4x, b=%#.4x, addr=%#.4x", args.a, args.b, args.addr);
}

template <>
void
disassemble_args<Instructions::Code::CJUMP_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::CJUMP_W> args)
{
  printf("CJUMP_W a=%#.4x, b=%#.4x, addr=%#.4x", args.a, args.b, args.addr);
}

template <>
void
disassemble_args<Instructions::Code::CMP>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::CMP> args)
{
  printf("CMP a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::CMP_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::CMP_W> args)
{
  printf("CMP_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::SET_V>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::SET_V> args)
{
  printf("SET_V addr=%#.4x, value=%#.4x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::SET_VW>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::SET_VW> args)
{
  printf("SET_VW addr=%#.4x, value=%#.4x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::COPY>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::COPY> args)
{
  printf("COPY from=%#.4x, to=%#.4x", args.from, args.to);
}

template <>
void
disassemble_args<Instructions::Code::COPY_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::COPY_W> args)
{
  printf("COPY_W from=%#.4x, to=%#.4x", args.from, args.to);
}

template <>
void
disassemble_args<Instructions::Code::GET>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::GET> args)
{
  printf("GET from_p=%#.4x, to=%#.4x", args.from_p, args.to);
}

template <>
void
disassemble_args<Instructions::Code::GET_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::GET_W> args)
{
  printf("GET_W from_p=%#.4x, to=%#.4x", args.from_p, args.to);
}

template <>
void
disassemble_args<Instructions::Code::SET>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::SET> args)
{
  printf("SET from=%#.4x, to_p=%#.4x", args.from, args.to_p);
}

template <>
void
disassemble_args<Instructions::Code::SET_W>(Machine::Machine const & machine, Instructions::Args<Instructions::Code::SET_W> args)
{
  printf("GET_W from=%#.4x, to_p=%#.4x", args.from, args.to_p);
}


template <Instructions::Code Code>
void
disassemble_instruction_templated(Machine::Machine const & machine, Machine::MemoryAddress& addr)
{
  Instructions::Args<Code> args = Machine::advance_addr<Instructions::Args<Code>>(machine, addr);

  printf("+%lu) :  ", sizeof(Instructions::Args<Code>));

  disassemble_args<Code>(machine, args);
}


Machine::MemoryAddress
disassemble_instruction(Machine::Machine const & machine, Machine::MemoryAddress addr)
{
  printf("%#.4x (%lu", addr, sizeof(Instructions::Code));
  Instructions::Code code = Machine::advance_addr<Instructions::Code>(machine, addr);
  switch (code)
  {
    case (Instructions::Code::NOP):  printf(") :  NOP");
      break;
    case (Instructions::Code::ADD):  disassemble_instruction_templated<Instructions::Code::ADD>(machine, addr);
      break;
    case (Instructions::Code::ADD_W):  disassemble_instruction_templated<Instructions::Code::ADD_W>(machine, addr);
      break;
    case (Instructions::Code::SUB):  disassemble_instruction_templated<Instructions::Code::SUB>(machine, addr);
      break;
    case (Instructions::Code::SUB_W):  disassemble_instruction_templated<Instructions::Code::SUB_W>(machine, addr);
      break;
    case (Instructions::Code::MUL):  disassemble_instruction_templated<Instructions::Code::MUL>(machine, addr);
      break;
    case (Instructions::Code::MUL_W):  disassemble_instruction_templated<Instructions::Code::MUL_W>(machine, addr);
      break;
    case (Instructions::Code::DIV):  disassemble_instruction_templated<Instructions::Code::DIV>(machine, addr);
      break;
    case (Instructions::Code::DIV_W):  disassemble_instruction_templated<Instructions::Code::DIV_W>(machine, addr);
      break;
    case (Instructions::Code::NOT):  disassemble_instruction_templated<Instructions::Code::NOT>(machine, addr);
      break;
    case (Instructions::Code::LSHIFT):  disassemble_instruction_templated<Instructions::Code::LSHIFT>(machine, addr);
      break;
    case (Instructions::Code::RSHIFT):  disassemble_instruction_templated<Instructions::Code::RSHIFT>(machine, addr);
      break;
    case (Instructions::Code::AND):  disassemble_instruction_templated<Instructions::Code::AND>(machine, addr);
      break;
    case (Instructions::Code::OR):  disassemble_instruction_templated<Instructions::Code::OR>(machine, addr);
      break;
    case (Instructions::Code::XOR):  disassemble_instruction_templated<Instructions::Code::XOR>(machine, addr);
      break;
    case (Instructions::Code::JUMP_V):  disassemble_instruction_templated<Instructions::Code::JUMP_V>(machine, addr);
      break;
    case (Instructions::Code::JUMP):  disassemble_instruction_templated<Instructions::Code::JUMP>(machine, addr);
      break;
    case (Instructions::Code::CJUMP):  disassemble_instruction_templated<Instructions::Code::CJUMP>(machine, addr);
      break;
    case (Instructions::Code::CJUMP_W):  disassemble_instruction_templated<Instructions::Code::CJUMP_W>(machine, addr);
      break;
    case (Instructions::Code::CMP):  disassemble_instruction_templated<Instructions::Code::CMP>(machine, addr);
      break;
    case (Instructions::Code::CMP_W):  disassemble_instruction_templated<Instructions::Code::CMP_W>(machine, addr);
      break;
    case (Instructions::Code::SET_V):  disassemble_instruction_templated<Instructions::Code::SET_V>(machine, addr);
      break;
    case (Instructions::Code::SET_VW):  disassemble_instruction_templated<Instructions::Code::SET_VW>(machine, addr);
      break;
    case (Instructions::Code::COPY):  disassemble_instruction_templated<Instructions::Code::COPY>(machine, addr);
      break;
    case (Instructions::Code::COPY_W):  disassemble_instruction_templated<Instructions::Code::COPY_W>(machine, addr);
      break;
    case (Instructions::Code::GET):  disassemble_instruction_templated<Instructions::Code::GET>(machine, addr);
      break;
    case (Instructions::Code::GET_W):  disassemble_instruction_templated<Instructions::Code::GET_W>(machine, addr);
      break;
    case (Instructions::Code::SET):  disassemble_instruction_templated<Instructions::Code::SET>(machine, addr);
      break;
    case (Instructions::Code::SET_W):  disassemble_instruction_templated<Instructions::Code::SET_W>(machine, addr);
      break;

    default:
      printf(") Unrecognised code: %#.2hhx", code);
  }
  printf("\n");

  return addr;
}


void
disassemble(Machine::Machine const & machine, Machine::MemoryAddress from, Machine::MemoryAddress to, char const* prefix)
{
  Machine::MemoryAddress addr = from;
  while (addr < to)
  {
    if (prefix != NULL)
    {
      printf("%s", prefix);
    }
    addr = disassemble_instruction(machine, addr);
    assert(addr <= to);
  }
}

} // namespace Disassembler