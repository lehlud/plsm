#pragma once

#include "AST/Base.h"
#include <string>
#include <memory>

namespace ast
{
  class UnaryExpr : public Expr
  {
    const std::string op;
    const std::unique_ptr<Expr> expr;

  public:
    UnaryExpr(const std::string &op, std::unique_ptr<Expr> &expr)
        : op(op), expr(std::move(expr)) {}
  };
}