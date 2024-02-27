#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class NamedType : public Type
  {
  public:
    NamedType(const std::string &name) : name(name) {}
    const std::string name;
  };
}