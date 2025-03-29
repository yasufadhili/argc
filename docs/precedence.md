# Expression Parser Documentation

## Overview
This document provides a comprehensive reference for the expression parser implemented in our language. The parser follows a recursive descent approach with support for various expression types and a well-defined operator precedence hierarchy.

## Supported Expression Types

### Literal Expressions
- **Integer Literals**: Numeric values (e.g., `42`, `123`, `0`)

### Grouping Expressions
- **Parenthesised Expressions**: Expressions enclosed in parentheses to control evaluation order (e.g., `(1 + 2)`)

### Unary Expressions
- **Bitwise NOT**: `~` operator (e.g., `~42`)
- **Negation**: `-` operator (e.g., `-42`)

### Binary Expressions

#### Arithmetic Operators
- **Multiplication**: `*` (e.g., `a * b`)
- **Division**: `/` (e.g., `a / b`)
- **Modulo**: `%` (e.g., `a % b`)
- **Addition**: `+` (e.g., `a + b`)
- **Subtraction**: `-` (e.g., `a - b`)

#### Bitwise Operators
- **Left Shift**: `<<` (e.g., `a << 2`)
- **Right Shift**: `>>` (e.g., `a >> 2`)
- **Bitwise AND**: `&` (e.g., `a & b`)
- **Bitwise XOR**: `^` (e.g., `a ^ b`)
- **Bitwise OR**: `|` (e.g., `a | b`)

#### Comparison Operators
- **Less Than**: `<` (e.g., `a < b`)
- **Greater Than**: `>` (e.g., `a > b`)
- **Less Than or Equal**: `<=` (e.g., `a <= b`)
- **Greater Than or Equal**: `>=` (e.g., `a >= b`)

#### Equality Operators
- **Equal**: `==` (e.g., `a == b`)
- **Not Equal**: `!=` (e.g., `a != b`)

#### Logical Operators
- **Logical AND**: `&&` (e.g., `a && b`)
- **Logical OR**: `||` (e.g., `a || b`)

### Ternary Expressions
- **Conditional Expression**: `condition ? expr1 : expr2`
    - If `condition` evaluates to true, the result is `expr1`
    - Otherwise, the result is `expr2`

## Operator Precedence

The parser implements the following precedence rules (listed from highest to lowest):

| Level | Operators | Description | Associativity |
|-------|-----------|-------------|---------------|
| 1 | Literals, Parentheses | Base expressions | N/A |
| 2 | `~`, `-` (unary) | Unary operators | Right-to-left |
| 3 | `*`, `/`, `%` | Multiplication, division, modulo | Left-to-right |
| 4 | `+`, `-` | Addition, subtraction | Left-to-right |
| 5 | `<<`, `>>` | Bit shifting | Left-to-right |
| 6 | `<`, `>`, `<=`, `>=` | Comparison | Left-to-right |
| 7 | `==`, `!=` | Equality | Left-to-right |
| 8 | `&` | Bitwise AND | Left-to-right |
| 9 | `^` | Bitwise XOR | Left-to-right |
| 10 | `\|` | Bitwise OR | Left-to-right |
| 11 | `&&` | Logical AND | Left-to-right |
| 12 | `\|\|` | Logical OR | Left-to-right |
| 13 | `?:` | Ternary conditional | Right-to-left |

## Grammar Definition

The expression grammar is formally defined as follows:

```
Expression    → TernaryExpr
TernaryExpr   → LogicalOrExpr ("?" Expression ":" TernaryExpr)?
LogicalOrExpr → LogicalAndExpr ("||" LogicalAndExpr)*
LogicalAndExpr → BitwiseOrExpr ("&&" BitwiseOrExpr)*
BitwiseOrExpr → BitwiseXorExpr ("|" BitwiseXorExpr)*
BitwiseXorExpr → BitwiseAndExpr ("^" BitwiseAndExpr)*
BitwiseAndExpr → EqualityExpr ("&" EqualityExpr)*
EqualityExpr  → ComparisonExpr (("==" | "!=") ComparisonExpr)*
ComparisonExpr → ShiftExpr (("<" | ">" | "<=" | ">=") ShiftExpr)*
ShiftExpr     → TermExpr (("<<" | ">>") TermExpr)*
TermExpr      → FactorExpr (("+" | "-") FactorExpr)*
FactorExpr    → UnaryExpr (("*" | "/" | "%") UnaryExpr)*
UnaryExpr     → ("~" | "-") UnaryExpr | PrimaryExpr
PrimaryExpr   → INTEGER | "(" Expression ")"
```

## Example Expressions

The following examples demonstrate the evaluation order based on the precedence rules:

1. **Basic Arithmetic**: `3 + 4 * 2` evaluates to `11` (multiplication before addition)

2. **Parenthesised Expressions**: `(3 + 4) * 2` evaluates to `14` (parentheses override default precedence)

3. **Bitwise Operations**: `1 | 2 & 3` evaluates to `3` (bitwise AND before bitwise OR)

4. **Mixed Operations**: `5 + 3 * 2 > 10 && 7 <= 10` evaluates to `true`
    - `3 * 2` = `6`
    - `5 + 6` = `11`
    - `11 > 10` = `true`
    - `7 <= 10` = `true`
    - `true && true` = `true`

5. **Ternary Expression**: `a > b ? a : b` (returns the larger of a and b)

6. **Complex Expression**: `~(1 << 2) & (3 + 4) | 5 ^ 6`
    - `1 << 2` = `4`
    - `~4` = `-5` (bitwise complement)
    - `3 + 4` = `7`
    - `-5 & 7` = `3`
    - `5 ^ 6` = `3`
    - `3 | 3` = `3`

## Implementation Notes

- The parser implements each grammar production as a separate function
- Each function correctly handles operator precedence and associativity
- Error reporting includes the position and lexeme for improved debugging
- The parser handles all expression types with properly nested AST nodes

## Common Errors

- **Mismatched parentheses**: Missing closing parenthesis will result in a parse error
- **Unexpected tokens**: The parser expects specific token types at various stages
- **Incomplete expressions**: Expressions must be complete and well-formed