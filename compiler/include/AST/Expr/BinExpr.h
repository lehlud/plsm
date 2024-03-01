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
    BinExpr(LOC_ARG, std::unique_ptr<Expr> &left, const std::string &op, std::unique_ptr<Expr> &right)
        : Expr(location), left(std::move(left)), op(op), right(std::move(right)) {}

    virtual std::string str(size_t indent, size_t tabstop)
    {
      return (std::stringstream() << "BinExpr("
                                  << "op=\"" << op << "\", "
                                  << "left=" << left->str() << ", "
                                  << "right=" << right->str()
                                  << ")")
          .str();
    }
  };

  class PrefExpr : public Expr
  {
    const std::unique_ptr<Expr> expr;

  public:
    PrefExpr(LOC_ARG, std::unique_ptr<Expr> &expr)
        : Expr(location), expr(std::move(expr)) {}

    virtual std::string str()
    {
      return (std::stringstream() << "PrefExpr("
                                  << "expr=" << expr->str()
                                  << ")")
          .str();
    }
  };
}