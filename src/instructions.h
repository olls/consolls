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
struct __attribute__((packed)) COPY
{
  Machine::MemoryAddress from;
  Machine::MemoryAddress to;
};


// Compile time mappings from 'args' struct types to corresponding Code types
template <typename ArgsType>
static const Instructions::Code Args_to_Code = Instructions::Code::NOP;

template<>
static const Instructions::Code Args_to_Code<Instructions::NOP> = Instructions::Code::NOP;

template<>
static const Instructions::Code Args_to_Code<Instructions::ADD<u8>> = Instructions::Code::ADD;
template<>
static const Instructions::Code Args_to_Code<Instructions::ADD<u16>> = Instructions::Code::ADD_W;

template<>
static const Instructions::Code Args_to_Code<Instructions::SUB<u8>> = Instructions::Code::SUB;
template<>
static const Instructions::Code Args_to_Code<Instructions::SUB<u16>> = Instructions::Code::SUB_W;

template<>
static const Instructions::Code Args_to_Code<Instructions::MUL<u8>> = Instructions::Code::MUL;
template<>
static const Instructions::Code Args_to_Code<Instructions::MUL<u16>> = Instructions::Code::MUL_W;

template<>
static const Instructions::Code Args_to_Code<Instructions::DIV<u8>> = Instructions::Code::DIV;
template<>
static const Instructions::Code Args_to_Code<Instructions::DIV<u16>> = Instructions::Code::DIV_W;

template<>
static const Instructions::Code Args_to_Code<Instructions::JUMP> = Instructions::Code::JUMP;

template<>
static const Instructions::Code Args_to_Code<Instructions::CMP<u8>> = Instructions::Code::CMP;
template<>
static const Instructions::Code Args_to_Code<Instructions::CMP<u16>> = Instructions::Code::CMP_W;

template<>
static const Instructions::Code Args_to_Code<Instructions::SET<u8>> = Instructions::Code::SET;
template<>
static const Instructions::Code Args_to_Code<Instructions::SET<u16>> = Instructions::Code::SET_W;

template<>
static const Instructions::Code Args_to_Code<Instructions::COPY<u8>> = Instructions::Code::COPY;
template<>
static const Instructions::Code Args_to_Code<Instructions::COPY<u16>> = Instructions::Code::COPY_W;


#undef GEN_OP_IN

} // namespace Instructions