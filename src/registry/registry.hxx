#pragma once

#include <string>
#include <unordered_map>

namespace registry {
  class TypeRegistry {
    std::unordered_map<std::string, size_t> data_types;

  public:
    TypeRegistry() {
      add_data_type("i8", 1);
      add_data_type("i16", 2);
      add_data_type("i32", 4);
      add_data_type("i64", 8);
    };

    static auto get_instance() -> TypeRegistry & {
      static TypeRegistry instance;
      return instance;
    }

    auto add_data_type(const std::string &name, const size_t size_bytes) -> void {
      data_types[name] = size_bytes;
    }

    auto get_data_type_size(const std::string &name) const -> size_t {
      return data_types.at(name);
    }

    auto is_data_type_registered(const std::string &name) const -> bool {
      return data_types.contains(name);
    }

  };
}