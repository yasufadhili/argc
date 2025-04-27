# Argon Syntax Specification

## Table of Contents
1. [Lexical Elements](#lexical-elements)
2. [Types](#types)
3. [Expressions](#expressions)
4. [Statements](#statements)
5. [Functions](#functions)
6. [Modules](#modules)
7. [Memory Management](#memory-management)

## Lexical Elements

### Keywords
```argon
break        default      def         interface    select
case         defer        struct
else         goto         module
const        fallthrough  if           type
continue     for          import       return       var
repeat       match        put          while        nil
where        as           exit         enum         throws
guard        super        is           in           rune
```

### Operators
```argon
// Arithmetic
+    -    *    /    %    ++    --

// Bitwise
&    |    ^    <<    >>    &^
&=   |=   ^=   <<=   >>=   &^=

// Logical
&&   ||   !

// Comparison
==   !=   <    <=   >    >=   ===   !==

// Assignment
=    +=   -=   *=   /=   %=

// Other
.    :    ,    ;    ->   <-   ...   ~
```

### Literals

#### Integer Literals
```argon
// Decimal
42
1_000_000

// Binary
0b1010
0B1100

// Octal
0o755
0O644

// Hexadecimal
0xFF
0X1A
```

#### Floating-Point Literals
```argon
// Decimal
3.14
1.0e-10
1_000.000_1

// Hexadecimal
0x1.0p-10
0X1.0P+10
```

#### String Literals
```argon
// Interpreted strings
"Hello, World!\n"

// Raw strings
`Line 1
Line 2`
```

#### Rune Literals
```argon
'a'
'\n'
'\u0041'
'\U0001F600'
```

## Types

### Primitive Types
```argon
// Signed integers
i8    i16    i32    i64

// Unsigned integers
u8    u16    u32    u64

// Floating point
f32   f64

// String
str

// Boolean
bool

// Rune (Unicode code point)
rune
```

### Composite Types

#### Arrays
```argon
// Fixed-size array
[3]i32    // Array of 3 i32 elements

// Determined by compiler
[]i32     // Slice of i32 elements
```

#### Structures
```argon
type Person struct {
    name str
    age i32
    isOnline bool
}
```

#### Enumerations
```argon
type ErrorCode enum {
    SEVERE
    MINOR
    WARNING
}
```

## Expressions

### Arithmetic Expressions
```argon
a + b
a - b
a * b
a / b
a % b
```

### Bitwise Expressions
```argon
a & b
a | b
a ^ b
a << b
a >> b
```

### Logical Expressions
```argon
a && b
a || b
!a
```

### Comparison Expressions
```argon
a == b
a != b
a < b
a <= b
a > b
a >= b
```

## Statements

### Control Flow

#### If Statement
```argon
if x > 3 {
    print "Greater"
} else if x == 3 {
    print "Equal"
} else {
    print "Less"
}
```

#### Match Statement
```argon
match x {
    case: 1 {
        print "One"
        break
    }
    case: 2 {
        print "Two"
        break
    }
    case: 3 {
        fallthrough
    }
    case: 4 {
        print "Four"
    }
}
```

#### Loop Statements
```argon
// For loop

var i i8 = 0
for i < 5> {
    print i
    i++
}

// Repeat loop
repeat 4 {
    print "Four times"
}

var x = 1
repeat {
    // do something
    x += 1
} while x < 5
```

## Functions

### Function Declaration
```argon
// Simple function
def greet(name str) str {
    return "Hello, " + name
}

// Multiple return values
def minMax(a i32, b i32) (i32, i32) {
    if a < b {
        return (a, b)
    }
    return (b, a)
}

// Error handling
def canThrow() throws str {
    // Function that can throw errors
}
```

### Function Calls
```argon
greet("World")
var min, max = minMax(5, 10)
```

## Modules

### Module Declaration
```argon
module main

import sys
import math
```

### Module Usage
```argon
sys::print("Hello")
math::sqrt(16)
```

## Memory Management

### Variable Declaration
```argon
// Mutable variables
var x i32 = 10
var y = 20

// Constants
const PI = 3.14159
```

### Memory Safety
```argon
// Safe pointer operations
var ptr *i32
ptr = &x
*ptr = 42

// Array bounds checking
var arr [3]i32
arr[0] = 1  // Valid
arr[3] = 4  // Runtime error
```

## Notes on x86_64 Assembly Generation

The Argon compiler generates x86_64 assembly with the following considerations:

1. All primitive types map directly to their x86_64 equivalents:
   - i8/i16/i32/i64 → byte/word/dword/qword
   - f32/f64 → float/double

2. Arrays and structures are laid out in memory with proper alignment:
   - Arrays are contiguous in memory
   - Structures follow x86_64 alignment rules

3. Function calls use the System V AMD64 ABI:
   - First 6 integer arguments in registers (RDI, RSI, RDX, RCX, R8, R9)
   - First 8 floating-point arguments in XMM registers
   - Additional arguments on the stack
   - Return values in RAX (or RAX:RDX for 128-bit values)

4. Memory management:
   - Stack-based allocation for local variables
   - Heap allocation for dynamic data structures
   - Automatic bounds checking for arrays

```
