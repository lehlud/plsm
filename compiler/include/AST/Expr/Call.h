#pragma once

#include "AST/Base.h"
#include <vector>

namespace ast
{
  class CallExpr : public Expr
  {
  public:
    CallExpr(const Expr *callee, const std::vector<Expr *> &args)
        : callee(callee), args(std::move(args)) {}

    ~CallExpr()
    {
      delete callee;
      for (auto &arg : args)
        delete arg;
    }

    const Expr *callee;
    const std::vector<Expr *> args;
  };
}