#pragma once

#include "machine.h"

#include "utils/types.h"
#include "utils/mem-layout.h"


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
  JUMP_V,
  CJUMP,
  CJUMP_W,
  CMP,
  CMP_W,
  SET_V,
  SET_VW,
  COPY,
  COPY_W,
  GET,
  GET_W,
  SET,
  SET_W
};


template <Code InstructionCode>
struct PACKED Args;


template <>
struct PACKED Args<Code::ADD>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct PACKED Args<Code::ADD_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::SUB>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct PACKED Args<Code::SUB_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::MUL>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct PACKED Args<Code::MUL_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::DIV>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct PACKED Args<Code::DIV_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::NOT>
{
  Machine::MemoryAddress in;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::LSHIFT>
{
  Machine::MemoryAddress in;
  Machine::MemoryAddress bits;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::RSHIFT>
{
  Machine::MemoryAddress in;
  Machine::MemoryAddress bits;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::AND>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::OR>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::XOR>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::JUMP>
{
  Machine::MemoryAddress addr;
};

template <>
struct PACKED Args<Code::JUMP_V>
{
  Machine::MemoryAddress addr;
};


template <>
struct PACKED Args<Code::CJUMP>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress addr;
};

template <>
struct PACKED Args<Code::CJUMP_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress addr;
};


template <>
struct PACKED Args<Code::CMP>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};

template <>
struct PACKED Args<Code::CMP_W>
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <>
struct PACKED Args<Code::SET_V>
{
  Machine::MemoryAddress addr;
  u8 value;
};

template <>
struct PACKED Args<Code::SET_VW>
{
  Machine::MemoryAddress addr;
  u16 value;
};


template <>
struct PACKED Args<Code::COPY>
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};

template <>
struct PACKED Args<Code::COPY_W>
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};


template <>
struct PACKED Args<Code::GET>
{
  Machine::MemoryAddress from_p;
  Machine::MemoryAddress to;
};

template <>
struct PACKED Args<Code::GET_W>
{
  Machine::MemoryAddress from_p;
  Machine::MemoryAddress to;
};


template <>
struct PACKED Args<Code::SET>
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to_p;
};

template <>
struct PACKED Args<Code::SET_W>
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to_p;
};

} // namespace Instructions
