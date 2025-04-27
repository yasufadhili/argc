# Arithetic Expressions in Argon

> Borrowed from the Carbon Project

## Table of contents

-   [Overview](#overview)
-   [Precedence and associativit](#precedence-and-associativity)
-   [Built-in types](#built-in-types)
    -   [Integer types](#integer-types)
        -   [Overflow and other error conditions](#overflow-and-other-error-conditions)
    -   [Floating-point types](#floating-point-types)
    


## Overview

Carbon provides a conventional set of arithmetic operators:

```zig
var a i32 = 9
var b i32 = 3

var negation i32 = -a

var sum i32 = a + b

var difference i32 = a - b

var product i32 = a * b

var quotient i32 = a / b

var remainder i32 = a % b

```

## Precedence and associativity

Binary `+` and `-` can be freely mixed, and are left-associative.

```zig
// -2, same as `((1 - 2) + 3) - 4`.
var n i32 = 1 - 2 + 3 - 4
```

Binary `*` and `/` can be freely mixed, and are left-associative.

```
// 0.375, same as `((1.0 / 2.0) * 3.0) / 4.0`.
var m f32 = 1.0 / 2.0 * 3.0 / 4.0
```

Unary `-` has higher precedence than binary `*`, `/`, and `%`. Binary `*` and
`/` have higher precedence than binary `+` and `-`.

```
// 5, same as `(-1) + ((-2) * (-3))`.
var x i32 = -1 + -2 * -3

// Error, parentheses required: no precedence order between `+` and `%`.
var y i32 = 2 + 3 % 5
```

## Built-in types

For binary operators, if the operands have different built-in types, they are
converted as follows:

-   If the types are `uN` and `uM`, or they are `iN` and `iM`, the operands are
    converted to the larger type.
-   If one type is `iN` and the other type is `uM`, and `M` < `N`, the `uM`
    operand is converted to `iN`.
-   If one type is `fN` and the other type is `iM` or `uM`, and there is an implicit conversionfrom the integer
    type to `fN`, then the integer operand is converted to `fN`.

### Integer types
Signed and unsigned integer types support all the arithmetic operators.

Signed integer arithmetic produces the usual mathematical result. Unsigned
integer arithmetic in `uN` wraps around modulo 2<sup>`N`</sup>.

Division truncates towards zero. The result of the `%` operator is defined by
the equation `a % b == a - (a / b) * b`.

#### Overflow and other error conditions

Integer arithmetic is subject to two classes of problems for which an operation
has no representable result:

-   Overflow, where the resulting value is too large to be represented in the
    type, or, for `%`, when the implied multiplication overflows.
-   Division by zero.

Unsigned integer arithmetic cannot overflow, but division by zero can still occur.


**Note:** All arithmetic operators can overflow for signed integer types. For example, given a value `v: iN` that is the least possible value for its type, `-v`, `v + v`, `v - 1`, `v * 2`, `v / -1`, and `v % -1` all result in overflow.



### Floating-point types

Floating-point types support all the arithmetic operators other than `%`. Floating-point types in Argon have IEEE 754 semantics, use the round-to-nearest rounding mode, and do not set any floating-point exception state.

Because floating-point arithmetic follows IEEE 754 rules: overflow results in ±∞, and division by zero results in either ±∞ or, for 0.0 / 0.0, a quiet NaN.



