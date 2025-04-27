#pragma once


namespace op {

enum class Bitwise {
  AND, OR, NOT, SHL, SHR
};

enum class Binary {
  LOGICAL_AND, LOGICAL_OR,
  BITWISE_AND, BITWISE_OR, BITWISE_NAND, BITWISE_NOR, BITWISE_XOR, BITWISE_XNOR, BITWISE_SHL, BITWISE_SHR,
  PLUS, MINUS, MULTIPLY, DIVIDE,
  LESS_THAN, GREATER_THAN, EQUAL_TO, NOT_EQUAL, GREATER_THAN_OR_EQUAL, LESS_THAN_OR_EQUAL,
  LOGICAL_AND_EQUAL, LOGICAL_OR_EQUAL,
};

enum class Unary {
  NEGATE, NOT, INVERSE,
};

enum class Arithmetic {
  ADD, SUB, MUL, DIV, MOD
};

enum class Relational {
  GT, LT, GEQ, LEQ, EQ, NEQ
};

enum class Logical {
  AND, OR
};

}

