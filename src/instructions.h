#pragma once

#include "machine.h"


namespace Instructions
{

enum class Code : u8
{
  NOP,
  ADD,
  ADD_W,
  SUB,
  SUB_W,
  MUL,
  MUL_W,
  DIV,
  DIV_W,
  JUMP,
  JUMP_I,
  CMP,
  CMP_W,
  SET,
  SET_W,
  SET_I,
  SET_I_W,
  COPY,
  COPY_W,
  COPY_I,
  COPY_I_W
};

template <Code InstructionCode>
struct __attribute__((packed)) Args;


template <>
struct __attribute__((packed)) Args<Code::NOP>
{
};


template <>
struct __attribute__((packed)) Args<Code::ADD>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct __attribute__((packed)) Args<Code::ADD_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::SUB>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct __attribute__((packed)) Args<Code::SUB_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::MUL>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct __attribute__((packed)) Args<Code::MUL_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::DIV>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct __attribute__((packed)) Args<Code::DIV_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::JUMP>
{
  Machine::MemoryAddress addr;
};

template <>
struct __attribute__((packed)) Args<Code::JUMP_I>
{
  Machine::MemoryAddress addr;
};


template <>
struct __attribute__((packed)) Args<Code::CMP>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress addr;
};

template <>
struct __attribute__((packed)) Args<Code::CMP_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress addr;
};


template <>
struct __attribute__((packed)) Args<Code::SET>
{
  Machine::MemoryAddress addr;
  u8 value;
};

template <>
struct __attribute__((packed)) Args<Code::SET_W>
{
  Machine::MemoryAddress addr;
  u16 value;
};

template <>
struct __attribute__((packed)) Args<Code::SET_I>
{
  Machine::MemoryAddress addr;
  Machine::MemoryAddress value;
};

template <>
struct __attribute__((packed)) Args<Code::SET_I_W>
{
  Machine::MemoryAddress addr;
  Machine::MemoryAddress value;
};


template <>
struct __attribute__((packed)) Args<Code::COPY>
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};

template <>
struct __attribute__((packed)) Args<Code::COPY_W>
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};

template <>
struct __attribute__((packed)) Args<Code::COPY_I>
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};

template <>
struct __attribute__((packed)) Args<Code::COPY_I_W>
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};



inline u32
get_args_size(Code code)
{
  u32 result;

  switch (code)
  {
  case (Code::NOP):
    result = sizeof(Args<Code::NOP>);
    break;
  case (Code::ADD):
    result = sizeof(Args<Code::ADD>);
    break;
  case (Code::ADD_W):
    result = sizeof(Args<Code::ADD_W>);
    break;
  case (Code::SUB):
    result = sizeof(Args<Code::SUB>);
    break;
  case (Code::SUB_W):
    result = sizeof(Args<Code::SUB_W>);
    break;
  case (Code::MUL):
    result = sizeof(Args<Code::MUL>);
    break;
  case (Code::MUL_W):
    result = sizeof(Args<Code::MUL_W>);
    break;
  case (Code::DIV):
    result = sizeof(Args<Code::DIV>);
    break;
  case (Code::DIV_W):
    result = sizeof(Args<Code::DIV_W>);
    break;
  case (Code::JUMP):
    result = sizeof(Args<Code::JUMP>);
    break;
  case (Code::JUMP_I):
    result = sizeof(Args<Code::JUMP_I>);
    break;
  case (Code::CMP):
    result = sizeof(Args<Code::CMP>);
    break;
  case (Code::CMP_W):
    result = sizeof(Args<Code::CMP_W>);
    break;
  case (Code::SET):
    result = sizeof(Args<Code::SET>);
    break;
  case (Code::SET_W):
    result = sizeof(Args<Code::SET_W>);
    break;
  case (Code::SET_I):
    result = sizeof(Args<Code::SET_I>);
    break;
  case (Code::SET_I_W):
    result = sizeof(Args<Code::SET_I_W>);
    break;
  case (Code::COPY):
    result = sizeof(Args<Code::COPY>);
    break;
  case (Code::COPY_W):
    result = sizeof(Args<Code::COPY_W>);
    break;
  case (Code::COPY_I):
    result = sizeof(Args<Code::COPY_I>);
    break;
  case (Code::COPY_I_W):
    result = sizeof(Args<Code::COPY_I_W>);
    break;
  }
  return result;
}


#undef GEN_OP_IN

} // namespace Instructions