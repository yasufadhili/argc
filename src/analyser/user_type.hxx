#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <optional>

namespace analyser {

class UserDefinedType {
public:
    enum class Kind {
        STRUCT,
        ENUM,
        ALIAS,
        INTERFACE
    };

    struct Field {
        std::string name;
        std::string type_name;
        
        Field(std::string name, std::string type_name)
            : name(std::move(name)), type_name(std::move(type_name)) {}
    };

private:
    std::string name_;
    Kind kind_;
    std::vector<Field> fields_;
    std::optional<std::string> base_type_name_; // For aliases

public:
    UserDefinedType(std::string name, Kind kind)
        : name_(std::move(name)), kind_(kind) {}

    UserDefinedType(std::string name, std::string base_type_name)
        : name_(std::move(name)), kind_(Kind::ALIAS), base_type_name_(std::move(base_type_name)) {}

    void add_field(const std::string& name, const std::string& type_name) {
        fields_.emplace_back(name, type_name);
    }

    [[nodiscard]] const std::string& name() const { return name_; }
    [[nodiscard]] Kind kind() const { return kind_; }
    [[nodiscard]] const std::vector<Field>& fields() const { return fields_; }
    [[nodiscard]] const std::optional<std::string>& base_type_name() const { return base_type_name_; }

    [[nodiscard]] bool has_field(const std::string& field_name) const {
        for (const auto& field : fields_) {
            if (field.name == field_name) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] std::optional<Field> get_field(const std::string& field_name) const {
        for (const auto& field : fields_) {
            if (field.name == field_name) {
                return field;
            }
        }
        return std::nullopt;
    }
};

class TypeRegistry {
private:
    std::unordered_map<std::string, std::shared_ptr<UserDefinedType>> types_;

public:
    TypeRegistry() {
        // Register built-in types
        register_primitive_type("i8");
        register_primitive_type("i16");
        register_primitive_type("i32");
        register_primitive_type("i64");
        register_primitive_type("u8");
        register_primitive_type("u16");
        register_primitive_type("u32");
        register_primitive_type("u64");
        register_primitive_type("f32");
        register_primitive_type("f64");
        register_primitive_type("bool");
        register_primitive_type("void");
    }

    void register_primitive_type(const std::string& name) {
        auto type = std::make_shared<UserDefinedType>(name, UserDefinedType::Kind::ALIAS);
        types_[name] = type;
    }

    void register_type(std::shared_ptr<UserDefinedType> type) {
        types_[type->name()] = std::move(type);
    }

    [[nodiscard]] bool is_type_defined(const std::string& name) const {
        return types_.find(name) != types_.end();
    }

    [[nodiscard]] std::optional<std::shared_ptr<UserDefinedType>> get_type(const std::string& name) const {
        auto it = types_.find(name);
        if (it != types_.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

} // namespace analyser
