#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class Identifier : public Expr
  {
    const std::string name;

  public:
    Identifier(LOC_ARG, const std::string &name)
        : Expr(location), name(name) {}

    virtual std::string str()
    {
      return "Identifier(" + name + ")";
    }
  };
}