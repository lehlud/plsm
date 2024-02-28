#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class NamedType : public Type
  {
    const std::string name;

  public:
    NamedType(const std::string &name) : name(name) {}
  };
}