#pragma once


namespace sym {


enum class TypeKind {
  PRIMITIVE, ARRAY, STRUCT,
};

class Type final {
  TypeKind kind;
public:
  Type(TypeKind kind);
};


}

