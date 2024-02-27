#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class UnaryExpr : public Expr
  {
  public:
    UnaryExpr(const std::string &op, const Expr *expr)
        : op(op), expr(expr) {}

    ~UnaryExpr() { delete expr; }

    const std::string op;
    const Expr *expr;
  };
}