#pragma once

#include "AST/Base.h"

namespace ast
{
  class ExprStmt : public Stmt
  {
  public:
    ExprStmt(const Expr *expr) : expr(expr) {}
    ~ExprStmt() { delete expr; }

    const Expr *expr;
  };
}