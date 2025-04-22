#pragma once


namespace sym {


enum class TypeKind {
  PRIMITIVE, ARRAY, STRUCT,
};

class Type final {
  TypeKind kind;
public:
  explicit Type(TypeKind kind);
};


}

