#pragma once

#include "ast.hh"
#include <map>
#include <string>

namespace codegen {

  class X86_64AssemblyVisitor : public ast::Visitor {
    int stack_offset_ { 0 };
    std::map<std::string, int> var_offsets_;
  };

}