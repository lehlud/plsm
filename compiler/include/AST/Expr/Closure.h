#pragma once

#include "AST/Base.h"
#include <string>
#include <vector>

namespace ast
{
  class Closure : public Expr
  {
  public:
    Closure(const std::vector<std::string> &args, const Expr *body)
        : args(std::move(args)), body(body) {}
    ~Closure() { delete body; }

    const std::vector<std::string> args;
    const Expr *body;
  };
}