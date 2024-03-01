#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class NamedType : public Type
  {
    const std::string name;

  public:
    NamedType(LOC_ARG, const std::string &name)
        : Type(location), name(name) {}
  };
}