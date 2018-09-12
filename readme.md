# Instructions

  NOP


## Arithmetic

  ADD
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress result;

  ADD_W
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress result;


  SUB
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress result;

  SUB_W
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress result;


  MUL
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress result;

  MUL_W
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress result;


  DIV
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress result;

  DIV_W
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress result;


## Branching

  JUMP
    MemoryAddress addr;

  JUMP_I
    MemoryAddress addr;


  CMP
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress addr;

  CMP_W
    MemoryAddress a;
    MemoryAddress b;
    MemoryAddress addr;


## Data

  SET
    MemoryAddress addr;
    u8 value;

  SET_W
    MemoryAddress addr;
    u16 value;

  SET_I
    MemoryAddress addr;
    MemoryAddress value;

  SET_I_W
    MemoryAddress addr;
    MemoryAddress value;


  COPY
    MemoryAddress from;
    MemoryAddress to;

  COPY_W
    MemoryAddress from;
    MemoryAddress to;

  COPY_I
    MemoryAddress from;
    MemoryAddress to;

  COPY_I_W
    MemoryAddress from;
    MemoryAddress to;
