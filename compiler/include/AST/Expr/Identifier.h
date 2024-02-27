#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class Identifier : public Expr
  {
  public:
    Identifier(const std::string &name) : name(name) {}

    const std::string name;
  };
}