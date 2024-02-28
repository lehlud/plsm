#pragma once

#include "AST/Base.h"
#include <vector>
#include <memory>

namespace ast
{
  class CallExpr : public Expr
  {
    const std::unique_ptr<Expr> callee;
    const std::vector<std::unique_ptr<Expr>> args;

  public:
    CallExpr(std::unique_ptr<Expr> &callee, std::vector<std::unique_ptr<Expr>> &args)
        : callee(std::move(callee)), args(std::move(args)) {}
  };
}