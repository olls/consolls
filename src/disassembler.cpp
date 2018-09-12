#include "disassembler.h"

#include "debug.h"
#include "instructions.h"

namespace Disassembler
{

template <Instructions::Code Code>
void
disassemble_args(Machine::Machine& machine, Instructions::Args<Code> args);

template <>
void
disassemble_args<Instructions::Code::ADD>(Machine::Machine& machine, Instructions::Args<Instructions::Code::ADD> args)
{
  printf("ADD a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::ADD_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::ADD_W> args)
{
  printf("ADD_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::SUB>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SUB> args)
{
  printf("SUB a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::SUB_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SUB_W> args)
{
  printf("SUB_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::MUL>(Machine::Machine& machine, Instructions::Args<Instructions::Code::MUL> args)
{
  printf("MUL a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::MUL_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::MUL_W> args)
{
  printf("MUL_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::DIV>(Machine::Machine& machine, Instructions::Args<Instructions::Code::DIV> args)
{
  printf("DIV a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::DIV_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::DIV_W> args)
{
  printf("DIV_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::JUMP>(Machine::Machine& machine, Instructions::Args<Instructions::Code::JUMP> args)
{
  printf("JUMP %#.4x", args.addr);
}

template <>
void
disassemble_args<Instructions::Code::JUMP_I>(Machine::Machine& machine, Instructions::Args<Instructions::Code::JUMP_I> args)
{
  printf("JUMP_I %#.4x", args.addr);
}

template <>
void
disassemble_args<Instructions::Code::CJUMP>(Machine::Machine& machine, Instructions::Args<Instructions::Code::CJUMP> args)
{
  printf("CJUMP a=%#.4x, b=%#.4x, addr=%#.4x", args.a, args.b, args.addr);
}

template <>
void
disassemble_args<Instructions::Code::CJUMP_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::CJUMP_W> args)
{
  printf("CJUMP_W a=%#.4x, b=%#.4x, addr=%#.4x", args.a, args.b, args.addr);
}

template <>
void
disassemble_args<Instructions::Code::CMP>(Machine::Machine& machine, Instructions::Args<Instructions::Code::CMP> args)
{
  printf("CMP a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::CMP_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::CMP_W> args)
{
  printf("CMP_W a=%#.4x, b=%#.4x, result=%#.4x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::SET>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SET> args)
{
  printf("SET addr=%#.4x, value=%#.4x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::SET_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SET_W> args)
{
  printf("SET_W addr=%#.4x, value=%#.4x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::SET_I>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SET_I> args)
{
  printf("SET_I addr=%#.4x, value=%#.4x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::SET_I_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SET_I_W> args)
{
  printf("SET_I_W addr=%#.4x, value=%#.4x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::COPY>(Machine::Machine& machine, Instructions::Args<Instructions::Code::COPY> args)
{
  printf("COPY from=%#.4x, to=%#.4x", args.from, args.to);
}

template <>
void
disassemble_args<Instructions::Code::COPY_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::COPY_W> args)
{
  printf("COPY_W from=%#.4x, to=%#.4x", args.from, args.to);
}

template <>
void
disassemble_args<Instructions::Code::COPY_I>(Machine::Machine& machine, Instructions::Args<Instructions::Code::COPY_I> args)
{
  printf("COPY_I from=%#.4x, to=%#.4x", args.from, args.to);
}

template <>
void
disassemble_args<Instructions::Code::COPY_I_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::COPY_I_W> args)
{
  printf("COPY_I_W from=%#.4x, to=%#.4x", args.from, args.to);
}



template <Instructions::Code Code>
void
disassemble_instruction(Machine::Machine& machine, Machine::MemoryAddress& addr)
{
  Instructions::Args<Code> args = Machine::advance_addr<Instructions::Args<Code>>(machine, addr);

  printf("+%lu) :  ", sizeof(Instructions::Args<Code>));

  disassemble_args<Code>(machine, args);
}


void
disassemble(Machine::Machine& machine, Machine::MemoryAddress from, Machine::MemoryAddress to)
{
  Machine::MemoryAddress addr = from;
  while (addr < to)
  {
    printf("%#.4x (%lu", addr, sizeof(Instructions::Code));
    Instructions::Code code = Machine::advance_addr<Instructions::Code>(machine, addr);
    switch (code)
    {
      case (Instructions::Code::NOP):  printf(") :  NOP");
        break;
      case (Instructions::Code::ADD):  disassemble_instruction<Instructions::Code::ADD>(machine, addr);
        break;
      case (Instructions::Code::ADD_W):  disassemble_instruction<Instructions::Code::ADD_W>(machine, addr);
        break;
      case (Instructions::Code::SUB):  disassemble_instruction<Instructions::Code::SUB>(machine, addr);
        break;
      case (Instructions::Code::SUB_W):  disassemble_instruction<Instructions::Code::SUB_W>(machine, addr);
        break;
      case (Instructions::Code::MUL):  disassemble_instruction<Instructions::Code::MUL>(machine, addr);
        break;
      case (Instructions::Code::MUL_W):  disassemble_instruction<Instructions::Code::MUL_W>(machine, addr);
        break;
      case (Instructions::Code::DIV):  disassemble_instruction<Instructions::Code::DIV>(machine, addr);
        break;
      case (Instructions::Code::DIV_W):  disassemble_instruction<Instructions::Code::DIV_W>(machine, addr);
        break;
      case (Instructions::Code::JUMP):  disassemble_instruction<Instructions::Code::JUMP>(machine, addr);
        break;
      case (Instructions::Code::JUMP_I):  disassemble_instruction<Instructions::Code::JUMP_I>(machine, addr);
        break;
      case (Instructions::Code::CJUMP):  disassemble_instruction<Instructions::Code::CJUMP>(machine, addr);
        break;
      case (Instructions::Code::CJUMP_W):  disassemble_instruction<Instructions::Code::CJUMP_W>(machine, addr);
        break;
      case (Instructions::Code::CMP):  disassemble_instruction<Instructions::Code::CMP>(machine, addr);
        break;
      case (Instructions::Code::CMP_W):  disassemble_instruction<Instructions::Code::CMP_W>(machine, addr);
        break;
      case (Instructions::Code::SET):  disassemble_instruction<Instructions::Code::SET>(machine, addr);
        break;
      case (Instructions::Code::SET_W):  disassemble_instruction<Instructions::Code::SET_W>(machine, addr);
        break;
      case (Instructions::Code::SET_I):  disassemble_instruction<Instructions::Code::SET_I>(machine, addr);
        break;
      case (Instructions::Code::SET_I_W):  disassemble_instruction<Instructions::Code::SET_I_W>(machine, addr);
        break;
      case (Instructions::Code::COPY):  disassemble_instruction<Instructions::Code::COPY>(machine, addr);
        break;
      case (Instructions::Code::COPY_W):  disassemble_instruction<Instructions::Code::COPY_W>(machine, addr);
        break;
      case (Instructions::Code::COPY_I):  disassemble_instruction<Instructions::Code::COPY_I>(machine, addr);
        break;
      case (Instructions::Code::COPY_I_W):  disassemble_instruction<Instructions::Code::COPY_I_W>(machine, addr);
        break;

      default:
        printf(") Unrecognised code: %#.2hhx", code);
    }
    printf("\n");

    assert(addr <= to);
  }
}

} // namespace Disassembler