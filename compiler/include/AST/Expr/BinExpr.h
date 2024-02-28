#pragma once

#include "AST/Base.h"
#include <string>
#include <memory>

namespace ast
{
  class BinExpr : public Expr
  {
    const std::string op;
    const std::unique_ptr<Expr> left, right;

  public:
    BinExpr(std::unique_ptr<Expr> &left, const std::string &op, std::unique_ptr<Expr> &right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
  };

  class PrefExpr : public Expr
  {
    const std::unique_ptr<Expr> expr;

  public:
    PrefExpr(std::unique_ptr<Expr> &expr) : expr(std::move(expr)) {}
  };
}