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


struct __attribute__((packed)) NOP
{
};


template <typename width>
struct __attribute__((packed)) ADD
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <typename width>
struct __attribute__((packed)) SUB
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <typename width>
struct __attribute__((packed)) MUL
{
  Machine::MemoryAddress a;
  Machine::MemoryAddress b;
  Machine::MemoryAddress result;
};


template <typename width>
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

struct __attribute__((packed)) JUMP_I
{
  Machine::MemoryAddress addr;
};


template <typename width>
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

template <typename width>
struct __attribute__((packed)) SET_I
{
  Machine::MemoryAddress addr;
  Machine::MemoryAddress value;
};


template <typename width>
struct __attribute__((packed)) COPY
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};

template <typename width>
struct __attribute__((packed)) COPY_I
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};


// Compile time mappings from 'args' struct types to corresponding Code types
template <typename ArgsType>
static const Code Args_to_Code = Code::NOP;

template<>
static const Code Args_to_Code<NOP> = Code::NOP;

template<>
static const Code Args_to_Code<ADD<u8>> = Code::ADD;
template<>
static const Code Args_to_Code<ADD<u16>> = Code::ADD_W;

template<>
static const Code Args_to_Code<SUB<u8>> = Code::SUB;
template<>
static const Code Args_to_Code<SUB<u16>> = Code::SUB_W;

template<>
static const Code Args_to_Code<MUL<u8>> = Code::MUL;
template<>
static const Code Args_to_Code<MUL<u16>> = Code::MUL_W;

template<>
static const Code Args_to_Code<DIV<u8>> = Code::DIV;
template<>
static const Code Args_to_Code<DIV<u16>> = Code::DIV_W;

template<>
static const Code Args_to_Code<JUMP> = Code::JUMP;
template<>
static const Code Args_to_Code<JUMP_I> = Code::JUMP_I;

template<>
static const Code Args_to_Code<CMP<u8>> = Code::CMP;
template<>
static const Code Args_to_Code<CMP<u16>> = Code::CMP_W;

template<>
static const Code Args_to_Code<SET<u8>> = Code::SET;
template<>
static const Code Args_to_Code<SET<u16>> = Code::SET_W;

template<>
static const Code Args_to_Code<SET_I<u8>> = Code::SET_I;
template<>
static const Code Args_to_Code<SET_I<u16>> = Code::SET_I_W;

template<>
static const Code Args_to_Code<COPY<u8>> = Code::COPY;
template<>
static const Code Args_to_Code<COPY<u16>> = Code::COPY_W;

template<>
static const Code Args_to_Code<COPY_I<u8>> = Code::COPY_I;
template<>
static const Code Args_to_Code<COPY_I<u16>> = Code::COPY_I_W;


#undef GEN_OP_IN

} // namespace Instructions