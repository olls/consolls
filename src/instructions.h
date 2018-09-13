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
  NOT,
  LSHIFT,
  RSHIFT,
  AND,
  OR,
  XOR,
  JUMP,
  JUMP_I,
  CJUMP,
  CJUMP_W,
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
struct __attribute__((packed)) Args<Code::NOT>
{
  Machine::MemoryAddress in;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::LSHIFT>
{
  Machine::MemoryAddress in;
  Machine::MemoryAddress bits;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::RSHIFT>
{
  Machine::MemoryAddress in;
  Machine::MemoryAddress bits;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::AND>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::OR>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct __attribute__((packed)) Args<Code::XOR>
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
struct __attribute__((packed)) Args<Code::CJUMP>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress addr;
};

template <>
struct __attribute__((packed)) Args<Code::CJUMP_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress addr;
};


template <>
struct __attribute__((packed)) Args<Code::CMP>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct __attribute__((packed)) Args<Code::CMP_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
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


#undef GEN_OP_IN

} // namespace Instructions