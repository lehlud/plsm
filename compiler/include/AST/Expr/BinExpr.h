#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>

namespace plsm {
namespace ast {

enum BinOp {
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,

  EQ,
  NE,
  GT,
  GE,
  LT,
  LE,

  AND,
  OR,
};

class BinExpr : public Expr {
  const BinOp op;
  const std::shared_ptr<Expr> lhs, rhs;

public:
  BinExpr(LOC_ARG, const BinOp op, Expr *lhs, Expr *rhs)
      : Expr(sourceRange), op(op), lhs(lhs), rhs(rhs) {}

  virtual boost::json::value toJson() override;
  static BinExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
