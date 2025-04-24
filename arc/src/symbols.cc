#include "include/symbols.hh"

using namespace sym;


Type::Type(const TypeKind kind, std::string name) : kind(kind), name(std::move(name)) {

}
