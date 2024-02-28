#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class Identifier : public Expr
  {
    const std::string name;

  public:
    Identifier(const std::string &name) : name(name) {}
  };
}