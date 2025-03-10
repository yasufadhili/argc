# Argon - A Hobby Programming Language

Argon is a **simple and minimalistic programming language** that compiles directly to **x86_64 Assembly** for Linux systems. 

This is a hobby project built purely out of curiosity and in exploration of low-level programming.

Argon has no fancy abstractions—just a straightforward way to write code that translates directly into machine instructions. If you enjoy tinkering with assembly but want a more readable syntax, this might be interesting to you!

## Features

- **Handcrafted simplicity**: No unnecessary features—just the basics for writing raw, efficient code.
- **Direct Compilation to Assembly**: Everything you write translates to assembly instructions with minimal overhead.
- **Explicit Memory Management**: No garbage collection, so you control how memory is allocated and freed.
- **Static Typing**: All variables must have explicitly defined types.
- **No Standard Library**: The only operations available are those that can be directly mapped to Assembly.

---

## Installation

This project is in its early stages, but if you want to play around with it, Then clone the repository and build the Argon compiler using `g++`:

```sh
$ git clone https://github.com/yasufadhili/Argon.git
$ cd Argon
$ make
```

This will generate the `argon` compiler executable.

## Usage

To compile an Argon source file:

```sh
$ argon source.ar
```

By default, this creates an executable named `out`.

To specify a custom output file:

```sh
$ argon source.ar -o my_app
```

---

## Syntax

### Example Program

```argon
let _start: i8 = set() {
  ret 0;
}
```

### Variables

```argon
let a: i32 = 3;
let b: i32;
b = 9;
let c: i32 = a + b;
```

---

## Data Types

Argon supports a set of primitive data types, which directly correspond to Assembly types.

<!--
| Type   | Name             | Size (bits) | Assembly Equivalent |
|--------|----------------------|------------|---------------------|
| `i8`   | Signed Byte          | 8          | `BYTE`              |
| `i16`  | Signed Word          | 16         | `WORD`              |
| `i32`  | Signed Double Word   | 32         | `DWORD`             |
| `i64`  | Signed Quad Word     | 64         | `QWORD`             |
| `f8`   | 8-bit Floating Point | 8          | `BYTE`              |
| `f16`  | 16-bit Floating Point | 16         | `WORD`              |
| `f32`  | 32-bit Floating Point | 32         | `DWORD`             |
| `f64`  | 64-bit Floating Point | 64         | `QWORD`             |
| `u8`   | Unsigned Byte        | 8          | `BYTE`              |
| `u16`  | Unsigned Word        | 16         | `WORD`              |
| `u32`  | Unsigned Double Word | 32         | `DWORD`             |
| `u64`  | Unsigned Quad Word   | 64         | `QWORD`             |
-->

| Type  | Size (bits) | Assembly Equivalent |
|-------|------------|---------------------|
| `i8`  | 8          | `BYTE`              |
| `i16` | 16         | `WORD`              |
| `i32` | 32         | `DWORD`             |
| `i64` | 64         | `QWORD`             |
| `f8`  | 8          | `BYTE`              |
| `f16` | 16         | `WORD`              |
| `f32` | 32         | `DWORD`             |
| `f64` | 64         | `QWORD`             |
| `u8`  | 8          | `BYTE`              |
| `u16` | 16         | `WORD`              |
| `u32` | 32         | `DWORD`             |
| `u64` | 64         | `QWORD`             |

---

## Operators

### Arithmetic Operators

| Operator | Description |
|----------|-------------|
| `+`      | Addition    |
| `-`      | Subtraction |
| `*`      | Multiplication |
| `/`      | Division |
| `%`      | Modulus |

### Logical Operators

| Operator | Description |
|----------|-------------|
| `&`      | Bitwise AND |
| `|`      | Bitwise OR  |
| `~`      | Bitwise NOT |
| `^`      | Bitwise XOR |

### Comparison Operators

| Operator | Description |
|----------|-------------|
| `==`     | Equal to    |
| `!=`     | Not equal to |
| `>`      | Greater than |
| `<`      | Less than |
| `>=`     | Greater than or equal to |
| `<=`     | Less than or equal to |

---

## Control Structures

### Conditionals

```argon
let _start: i8 = set() {
  let a: i32 = 5;
  if (a > 3) {
    puts "A is greater than 3";
  } else {
    puts "A is not greater than 3";
  }
  ret 0;
}
```

### Loops

#### While Loop

```argon
let _start: i8 = set() {
  let i: i32 = 0;
  while (i < 10) {
    puts i;
    i = i + 1;
  }
  ret 0;
}
```

#### Infinite Loop

```argon
let _start: i8 = set() {
  while (1) {
    puts "Running...";
  }
  ret 0;
}
```

---

## Functions

Functions in Argon are defined using `set()`, and they must return a specific data type.

```argon
let sum: i64 = set(x: i64, y: i64) {
  ret x + y;
}

let _start: i32 = set() {
  call sum(23, 90);
  let z: i64 = call sum(34, 78);
  puts z;
  ret 0;
}
```

---

## Memory Management

Argon does not have garbage collection, meaning all memory allocations must be handled manually.

### Allocating Memory

Memory can be allocated using `alloc(size)`, which returns a pointer to the allocated memory.

```argon
let ptr: i64* = alloc(64); // Allocate 64 bytes
```

### Writing to Memory

```argon
ptr[0] = 42;
ptr[1] = 99;
```

### Reading from Memory

```argon
let val: i32 = ptr[0];
puts val;
```

### Deallocating Memory

Memory should always be freed when it is no longer needed.

```argon
dealloc(ptr);
```

---

## Contributing

This is just a fun project, but if you have any ideas, feel free to fork the repo and mess around. PRs are welcome!

---

## License

This project is licensed under the MIT License.

