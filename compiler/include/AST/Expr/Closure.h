#pragma once

#include "AST/Base.h"
#include <string>
#include <vector>
#include <memory>

namespace ast
{
  class Closure : public Expr
  {
    const std::vector<std::string> args;
    const std::unique_ptr<Expr> body;

  public:
    Closure(std::vector<std::string> &args, std::unique_ptr<Expr> &body)
        : args(std::move(args)), body(std::move(body)) {}
  };
}