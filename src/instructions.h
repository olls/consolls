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
  CMP,
  CMP_W,
  SET,
  SET_W,
  COPY,
  COPY_W
};


struct __attribute__((packed)) NOP
{
};


struct __attribute__((packed)) ADD
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


struct __attribute__((packed)) SUB
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


struct __attribute__((packed)) MUL
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


struct __attribute__((packed)) DIV
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


struct __attribute__((packed)) JUMP
{
  Machine::MemoryAddress addr;
};


struct __attribute__((packed)) CMP
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress addr;
};


template <typename width>
struct __attribute__((packed)) SET
{
  Machine::MemoryAddress addr;
  width value;
};


struct __attribute__((packed)) COPY
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};


#undef GEN_OP_IN

} // namespace Instructions