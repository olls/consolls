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
disassemble_args<Instructions::Code::NOP>(Machine::Machine& machine, Instructions::Args<Instructions::Code::NOP> args)
{
  printf("NOP");
}

template <>
void
disassemble_args<Instructions::Code::ADD>(Machine::Machine& machine, Instructions::Args<Instructions::Code::ADD> args)
{
  printf("ADD a=%#x, b=%#x, result=%#x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::ADD_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::ADD_W> args)
{
  printf("ADD_W a=%#x, b=%#x, result=%#x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::SUB>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SUB> args)
{
  printf("SUB a=%#x, b=%#x, result=%#x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::SUB_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SUB_W> args)
{
  printf("SUB_W a=%#x, b=%#x, result=%#x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::MUL>(Machine::Machine& machine, Instructions::Args<Instructions::Code::MUL> args)
{
  printf("MUL a=%#x, b=%#x, result=%#x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::MUL_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::MUL_W> args)
{
  printf("MUL_W a=%#x, b=%#x, result=%#x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::DIV>(Machine::Machine& machine, Instructions::Args<Instructions::Code::DIV> args)
{
  printf("DIV a=%#x, b=%#x, result=%#x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::DIV_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::DIV_W> args)
{
  printf("DIV_W a=%#x, b=%#x, result=%#x", args.a, args.b, args.result);
}

template <>
void
disassemble_args<Instructions::Code::JUMP>(Machine::Machine& machine, Instructions::Args<Instructions::Code::JUMP> args)
{
  printf("JUMP %#x", args.addr);
}

template <>
void
disassemble_args<Instructions::Code::JUMP_I>(Machine::Machine& machine, Instructions::Args<Instructions::Code::JUMP_I> args)
{
  printf("JUMP_I %#x", args.addr);
}

template <>
void
disassemble_args<Instructions::Code::CMP>(Machine::Machine& machine, Instructions::Args<Instructions::Code::CMP> args)
{
  printf("CMP a=%#x, b=%#x, addr=%#x", args.a, args.b, args.addr);
}

template <>
void
disassemble_args<Instructions::Code::CMP_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::CMP_W> args)
{
  printf("CMP_W a=%#x, b=%#x, addr=%#x", args.a, args.b, args.addr);
}

template <>
void
disassemble_args<Instructions::Code::SET>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SET> args)
{
  printf("SET addr=%#x, value=%#x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::SET_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SET_W> args)
{
  printf("SET_W addr=%#x, value=%#x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::SET_I>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SET_I> args)
{
  printf("SET_I addr=%#x, value=%#x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::SET_I_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::SET_I_W> args)
{
  printf("SET_I_W addr=%#x, value=%#x", args.addr, args.value);
}

template <>
void
disassemble_args<Instructions::Code::COPY>(Machine::Machine& machine, Instructions::Args<Instructions::Code::COPY> args)
{
  printf("COPY from=%#x, to=%#x", args.from, args.to);
}

template <>
void
disassemble_args<Instructions::Code::COPY_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::COPY_W> args)
{
  printf("COPY_W from=%#x, to=%#x", args.from, args.to);
}

template <>
void
disassemble_args<Instructions::Code::COPY_I>(Machine::Machine& machine, Instructions::Args<Instructions::Code::COPY_I> args)
{
  printf("COPY_I from=%#x, to=%#x", args.from, args.to);
}

template <>
void
disassemble_args<Instructions::Code::COPY_I_W>(Machine::Machine& machine, Instructions::Args<Instructions::Code::COPY_I_W> args)
{
  printf("COPY_I_W from=%#x, to=%#x", args.from, args.to);
}



template <Instructions::Code Code>
void
disassemble_instruction(Machine::Machine& machine, Machine::MemoryAddress& addr)
{
  Instructions::Args<Code> args = Machine::advance_addr<Instructions::Args<Code>>(machine, addr);

  disassemble_args<Code>(machine, args);
}


void
disassemble(Machine::Machine& machine, Machine::MemoryAddress from, Machine::MemoryAddress to)
{
  for (Machine::MemoryAddress addr = from;
       addr < to;
       ++addr)
  {
    Instructions::Code code = Machine::get<Instructions::Code>(machine, addr);
    switch (code)
    {
      case (Instructions::Code::NOP):  disassemble_instruction<Instructions::Code::NOP>(machine, addr);
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
        printf("ERROR!");
    }
    printf("\n");

    assert(addr < to);
  }
}

} // namespace Disassembler