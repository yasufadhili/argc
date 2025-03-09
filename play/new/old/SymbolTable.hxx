#pragma once

#include <string>
#include <unordered_map>


class SymbolTable {

  // Stores Var Name, DataType, Address
  std::unordered_map<std::string, std::string, u_int32_t> vars_;

  // Stores Func name, return type, Address
  std::unordered_map<std::string, std::string, u_int32_t> funcs_;

public:

  static auto get_instance() -> SymbolTable& {
    static SymbolTable instance;
    return instance;
  }

};

