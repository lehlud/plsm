#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class BinExpr : public Expr
  {
  public:
    BinExpr(const Expr *left, const std::string &op, const Expr *right)
        : left(left), op(op), right(right) {}

    ~BinExpr()
    {
      delete left;
      delete right;
    }

    const Expr *left;
    const std::string op;
    const Expr *right;
  };

  class PrefExpr : public Expr
  {
  public:
    PrefExpr(const Expr *expr) : expr(expr) {}

    ~PrefExpr() { delete expr; }

    const Expr *expr;
  };
}