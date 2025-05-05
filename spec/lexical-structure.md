# Lexical Structure

## Table of Contents

1. [Overview](#overview)
2. [Source Code Representation](#source-code-representation)
3. [Lexical Elements](#lexical-elements)
   - [Comments](#comments)
   - [Tokens](#tokens)
   - [Whitespace](#whitespace)
4. [Identifiers](#identifiers)
   - [Keywords](#keywords)
   - [Predeclared Identifiers](#predeclared-identifiers)
   - [Naming Conventions](#naming-conventions)
5. [Keywords](#keywords-1)
   - [Reserved Keywords](#reserved-keywords)
   - [Contextual Keywords](#contextual-keywords)
6. [Operators and Punctuation](#operators-and-punctuation)
   - [Arithmetic Operators](#arithmetic-operators)
   - [Comparison Operators](#comparison-operators)
   - [Logical Operators](#logical-operators)
   - [Bitwise Operators](#bitwise-operators)
   - [Assignment Operators](#assignment-operators)
   - [Punctuation](#punctuation)
7. [Literals](#literals)
   - [Integer Literals](#integer-literals)
   - [Floating-Point Literals](#floating-point-literals)
   - [Character Literals](#character-literals)
   - [String Literals](#str-literals)
   - [Boolean Literals](#boolean-literals)
   - [Null Literal](#null-literal)
8. [References and Standards](#references-and-standards)

## Overview

The lexical structure of Argon defines how source code is broken down into tokens that can be processed by the compiler. This specification outlines the rules for writing valid Argon code at the character and token level, including comments, identifiers, keywords, operators, and literals.

## Source Code Representation

Argon source files are encoded in UTF-8, allowing for the full range of Unicode characters in source code. Source code is organised into files with the `.ar` extension, each representing a module in the Argon module system.

### Line Structure

Argon uses semicolons (`;`) as statement terminators. Unlike some languages, semicolons are not optional in Argon. This design choice emphasises clarity and parsability.

```
var x i8 = 5;
var y i8 = 10;
```

### Indentation

While Argon does not use significant indentation for syntactic purposes, a consistent indentation style of 2 or 4 spaces is recommended for readability. Tabs should not be used for indentation.

## Lexical Elements

### Comments

Argon supports both line comments and block comments:

```
// This is a line comment that extends to the end of the line

/* This is a block comment
   that can span multiple lines */

/// Documentation comment for the following item
```

Documentation comments (`///`) generate documentation when processed by Argon's documentation tool and follow a structured format detailed in the documentation tools specification.

### Tokens

Argon source code is divided into tokens, which are the smallest units of meaning in the language. Tokens include:

- Identifiers
- Keywords
- Literals
- Operators
- Punctuation

### Whitespace

Whitespace (spaces, tabs, newlines) is significant only as a token separator and is otherwise ignored by the compiler. However, newlines are significant for line comments and for improving error reporting.

## Identifiers

Identifiers name program entities such as variables, functions, types, modules, and more. They must follow these rules:

- Begin with a letter (a-z, A-Z) or underscore (_)
- Subsequent characters can be letters, digits (0-9), or underscores
- Case-sensitive (e.g., `value` and `Value` are different identifiers)
- Cannot be a keyword unless escaped with a prefix backtick (`` `if ``)

```
// Valid identifiers
x
count
snake_case
camelCase
PascalCase
_private
π
α_beta_γ

// Invalid identifiers
2fast      // Cannot start with a digit
for        // Reserved keyword (unless escaped as `for)
my-var     // Hyphens not allowed
```

### Keywords

Keywords cannot be used as identifiers without escaping.

### Predeclared Identifiers

Predeclared identifiers are built-in names that are not reserved. They can be redefined, but this is generally discouraged as it can lead to confusion.

```
// Predeclared identifiers include types and functions
i32
str
print
len
```

### Naming Conventions

Argon recommends these naming conventions:

- `snake_case` for variables, functions, and module names
- `PascalCase` for type names (structs, enums, interfaces)
- `SCREAMING_SNAKE_CASE` for constants
- `_leading_underscore` for private or internal entities

## Keywords

Keywords are reserved words that have special meaning in the language and cannot be used as identifiers except when escaped with backticks.

### Reserved Keywords

```
and         as          break       case
const       continue    default     defer        
else        enum        extern      fallthrough  
fn          for         goto        if
import      var         loop        repeat
match       mod         module      move         
not         null        or          ret      
shl         shr         match       throw
true        type        record      false
while       xor         catch 
```

### Contextual Keywords

Contextual keywords have special meaning only in specific contexts but can be used as identifiers elsewhere.

```
async       await       static      try
volatile    when
```

## Operators and Punctuation

Argon provides a comprehensive set of operators and punctuation symbols for expressing computations and controlling program flow.

### Arithmetic Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `+` | Addition | `a + b` |
| `-` | Subtraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |
| `%` | Modulo (remainder) | `a % b` |
| `**` | Exponentiation | `a ** b` |

### Comparison Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `==` | Equal | `a == b` |
| `!=` | Not equal | `a != b` |
| `<` | Less than | `a < b` |
| `>` | Greater than | `a > b` |
| `<=` | Less than or equal | `a <= b` |
| `>=` | Greater than or equal | `a >= b` |

### Logical Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `and` | Logical AND | `a and b` |
| `or` | Logical OR | `a or b` |
| `not` | Logical NOT | `not a` |

### Bitwise Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `&` | Bitwise AND | `a & b` |
| `|` | Bitwise OR | `a | b` |
| `^` | Bitwise XOR | `a ^ b` |
| `~` | Bitwise NOT | `~a` |
| `shl` | Left shift | `a shl b` |
| `shr` | Right shift | `a shr b` |
| `xor` | Bitwise XOR (keyword alternative) | `a xor b` |

### Assignment Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `=` | Assignment | `a = b` |
| `+=` | Add and assign | `a += b` |
| `-=` | Subtract and assign | `a -= b` |
| `*=` | Multiply and assign | `a *= b` |
| `/=` | Divide and assign | `a /= b` |
| `%=` | Modulo and assign | `a %= b` |
| `&=` | Bitwise AND and assign | `a &= b` |
| `|=` | Bitwise OR and assign | `a |= b` |
| `^=` | Bitwise XOR and assign | `a ^= b` |
| `<<=` | Left shift and assign | `a <<= b` |
| `>>=` | Right shift and assign | `a >>= b` |

### Punctuation

| Symbol | Description | Example |
|--------|-------------|---------|
| `;` | Statement terminator | `var x i16 = 5;` |
| `,` | Separator for elements | `fn f(a, b, c)` |
| `.` | Member access | `obj.field` |
| `:` | Type annotation | `var x: i32 = 5` |
| `::` | Module/scope resolution | `std::io::println` |
| `(` `)` | Grouping, function call | `f(x)` |
| `{` `}` | Block, compound statement | `{ var x i32 = 5; }` |
| `[` `]` | Indexing, array literals | `arr[i]` |
| `@` | Decorator/attribute | `@deprecated` |
| `#` | Directive | `#include` |
| `?` | Optional chaining | `obj?.field` |
| `!` | Macro invocation | `vector!` |
| `->` | Pointer access | `ptr->field` |
| `..` | Range (inclusive) | `1..5` |
| `...` | Range (exclusive) | `1...5` |
| `=>` | Lambda expression | `x => x + 1` |

## Literals

Literals are notations for representing fixed values in source code.

### Integer Literals

Integer literals can be written in decimal, hexadecimal, octal, or binary notation:

```
// Decimal (base 10)
42
1_000_000  // Underscores for readability

// Hexadecimal (base 16, prefix 0x)
0xFF
0xCAFE_BABE

// Octal (base 8, prefix 0o)
0o755
0o777_777

// Binary (base 2, prefix 0b)
0b1010
0b1101_0101
```

Integer literals can include a type suffix to specify their exact type:

```
42i8    // i8
42i16   // i16
42i32   // i32
42i64   // i64
42u8    // u8
42u16   // u16
42u32   // u32
42u64   // u64
```

Without a suffix, the type of an integer literal is inferred based on context, defaulting to `i32` if no constraints are present.

### Floating-Point Literals

Floating-point literals include a decimal point and optionally an exponent:

```
3.14159
1.0
0.5
1e10       // Scientific notation: 1 × 10¹⁰
1.5e-3     // 1.5 × 10⁻³
5_000.123  // Underscores for readability
```

Floating-point literals can include a type suffix:

```
3.14f32   // f32
3.14f64   // f64
```

Without a suffix, the type of a floating-point literal is inferred based on context, defaulting to `f64` if no constraints are present.

### Character Literals

Character literals represent Unicode code points and are enclosed in single quotes:

```
'a'       // Latin letter a
'λ'       // Greek letter lambda
'😊'      // Emoji
'\n'      // Newline escape sequence
'\u{1F642}'  // Unicode escape (🙂)
```

Escape sequences in character literals:

| Sequence | Meaning |
|----------|---------|
| `\'` | Single quote |
| `\"` | Double quote |
| `\\` | Backslash |
| `\n` | Newline |
| `\r` | Carriage ret |
| `\t` | Tab |
| `\b` | Backspace |
| `\f` | Form feed |
| `\0` | Null character |
| `\u{NNNN}` | Unicode code point (1-6 hex digits) |
| `\xNN` | ASCII character (2 hex digits) |

### String Literals

String literals represent sequences of characters and are enclosed in double quotes:

```
"Hello, world!"
"Line 1\nLine 2"
"Tab\tindented"
```

Argon supports several str literal forms:

1. **Regular str literals** - Enclosed in double quotes with escape processing
   ```
   "Hello, world!\n"
   ```

2. **Raw str literals** - Prefixed with `r` and do not process escapes
   ```
   r"C:\Program Files\Argon"
   ```

3. **Multi-line str literals** - Enclosed in triple double quotes
   ```
   """
   This is a multi-line
   str literal that preserves
   line breaks and indentation.
   """
   ```

4. **Interpolated str literals** - Prefixed with `f` and allow embedding expressions
   ```
   var name = "Argon";
   var greeting = f"Hello, {name}!";
   ```

### Boolean Literals

Boolean literals represent logical truth values:

```
true
false
```

### Null Literal

The `null` literal represents the absence of a value:

```
null
```

## References and Standards

Argon's lexical structure is informed by established practices from several languages while introducing unique elements:

- Unicode support follows [Unicode 14.0](https://unicode.org/versions/Unicode14.0.0/)

- UTF-8 encoding adheres to [RFC 3629](https://tools.ietf.org/html/rfc3629)
- Numeric literal formats draw inspiration from modern languages like Rust, Swift, and Go
- Identifier rules are compatible with common Unicode identifier practices

For further reading on lexical structures in programming languages:

- [Unicode Standard Annex #31: Unicode Identifier and Pattern Syntax](https://unicode.org/reports/tr31/)

- [Scanning and Parsing Modern Programming Languages](https://craftinginterpreters.com/scanning.html) by Robert Nystrom
- [Lexical Analysis](https://www.cs.cornell.edu/courses/cs4120/2022sp/notes/lexical-analysis.pdf) from Cornell University