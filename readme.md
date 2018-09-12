# Instructions

    NOP


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


## Branching

### JUMP
Sets the next instruction register equal to `addr`.

    MemoryAddress addr

### JUMP_I
Sets the next instruction register equal to the 16-bit address stored at `addr`.

    MemoryAddress addr


### CMP
Compares the 8-bit value stored at `a` to the value at `b`, is a is greater, set the next instruction register equal to `addr`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress addr

### CMP_W
Compares the 16-bit value stored at `a` to the value at `b`, is a is greater, set the next instruction register equal to `addr`.

    MemoryAddress a
    MemoryAddress b
    MemoryAddress addr


## Data

### SET
Sets the 8-bit value at `addr` equal to `value`.

    MemoryAddress addr
    u8 value

### SET_W
Sets the 16-bit value at `addr` equal to `value`.

    MemoryAddress addr
    u16 value

### SET_I
Sets the 8-bit value at the address stored in the address `addr`, to the value at the address `value`.

    MemoryAddress addr
    MemoryAddress value

### SET_I_W
Sets the 16-bit value at the address stored at the address `addr`, to the value at the address `value`.

    MemoryAddress addr
    MemoryAddress value


### COPY
Copies the 8-bit value at the address `from` to the value at `to`.

    MemoryAddress from
    MemoryAddress to

### COPY_W
Copies the 16-bit value at the address `from` to the value at `to`.

    MemoryAddress from
    MemoryAddress to

### COPY_I
Copies the 8-bit value at the address stored at the address `from`, to the value at the address stored at the address `to`.

    MemoryAddress from
    MemoryAddress to

### COPY_I_W
Copies the 16-bit value at the address stored at the address `from`, to the value at the address stored at the address `to`.

    MemoryAddress from
    MemoryAddress to
