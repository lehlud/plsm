#pragma once

#include "AST/Base.h"
#include <memory>

namespace ast
{
  class ExprStmt : public Stmt
  {
    const std::unique_ptr<Expr> expr;

  public:
    ExprStmt(LOC_ARG, std::unique_ptr<Expr> &expr)
        : Stmt(location), expr(std::move(expr)) {}
  };
}