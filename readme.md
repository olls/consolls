# Instructions

All instructions and arguments are byte aligned and packed sequentially into bytes.


## Misc

### NOP
Takes no arguments, and does nothing for 1 cycle.


## Arithmetic

### Add
Adds the 8-bit values at addresses `a` and `b` together, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result

### ADD_W
Adds the 16-bit values at addresses `a` and `b` together, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result


### SUB
Subtracts the 8-bit value at address `a` from the value at `b`, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result

### SUB_W
Subtracts the 16-bit value at address `a` from the value at `b`, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result


### MUL
Multiplies the 8-bit values at addresses `a` and `b` together, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result

### MUL_W
Multiplies the 16-bit values at addresses `a` and `b` together, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result


### DIV
Divides the 8-bit value at address `a` by the value at `b`, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result

### DIV_W
Divides the 16-bit value at address `a` by the value at `b`, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result


## Bitwise Logic

### NOT
Bitwise inversion of 8-bit value stored at `in`, storing the result at `result`.

    MemoryAddress in
    MemoryAddress result

### LSHIFT
Bitwise-left-shift of 8-bit value stored at `in` by the number of bits stored at `bits`, storing the result at `result`.

    MemoryAddress in
    MemoryAddress bits
    MemoryAddress result

### RSHIFT
Bitwise-right-shift of 8-bit value stored at `in` by the number of bits stored at `bits`, storing the result at `result`.

    MemoryAddress in
    MemoryAddress bits
    MemoryAddress result

### AND
Bitwise logical AND of 8-bit values stored at `a` and `b`, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result

### OR
Bitwise logical OR of 8-bit values stored at `a` and `b`, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result

### XOR
Bitwise logical exclusive OR of 8-bit values stored at `a` and `b`, storing the result at `result`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result


## Branching

### JUMP_V
Sets the next instruction register equal to `addr`.

    MemoryAddress addr

### JUMP
Sets the next instruction register equal to the 16-bit address stored at `addr_p`.

    MemoryAddress addr_p


## Comparison

### CJUMP
Compares the 8-bit value stored at `a` to the value at `b`, is a is greater, set the next instruction register equal to `addr`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress addr

### CJUMP_W
Compares the 16-bit value stored at `a` to the value at `b`, is a is greater, set the next instruction register equal to `addr`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress addr


### CMP
Compares the 8-bit value stored at `a` to the value at `b`, is they are equal, set the 8-bit value at `result` to `0x01`, otherwise `0x00`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result

### CMP_W
Compares the 16-bit value stored at `a` to the value at `b`, is they are equal, set the 8-bit value at `result` to `0x01`, otherwise `0x00`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress result


## Data

### SET_V
Sets the 8-bit value at `addr` equal to `value`.

    MemoryAddress addr
    u8 value

### SET_VW
Sets the 16-bit value at `addr` equal to `value`.

    MemoryAddress addr
    u16 value


### COPY
Copies the 8-bit value at the address `from` to the value at `to`.

    MemoryAddress from
    MemoryAddress to

### COPY_W
Copies the 16-bit value at the address `from` to the value at `to`.

    MemoryAddress from
    MemoryAddress to


### GET
Copies the 8-bit value at the address stored at `from_p` to the address `to`

    MemoryAddress from_p
    MemoryAddress to

### GET_W
Copies the 16-bit value at the address stored at `from_p` to the address `to`

    MemoryAddress from_p
    MemoryAddress to


### SET
Copies the 8-bit value at the address `from` to the address stored at `to_p`

    MemoryAddress from
    MemoryAddress to_p

### SET_W
Copies the 16-bit value at the address `from` to the address stored at `to_p`

    MemoryAddress from
    MemoryAddress to_p
