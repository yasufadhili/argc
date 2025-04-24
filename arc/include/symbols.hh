#pragma once

#include <string>

namespace sym {


enum class TypeKind {
  PRIMITIVE, ARRAY, STRUCT, BOOL
};

class Type final {
  TypeKind kind;
public:
  explicit Type(TypeKind kind, std::string name);
};


class Symbol {

};

}

