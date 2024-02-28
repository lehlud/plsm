#pragma once

#include "AST/Base.h"
#include <memory>

namespace ast
{
  class ExprStmt : public Stmt
  {
    const std::unique_ptr<Expr> expr;

  public:
    ExprStmt(std::unique_ptr<Expr> &expr) : expr(std::move(expr)) {}
  };
}