#pragma once

#include "AST/Base.h"
#include <memory>

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
public:
  const BinOp op;
  std::unique_ptr<Expr> lhs, rhs;

  BinExpr(LOC_ARG, const BinOp op, std::unique_ptr<Expr> lhs,
          std::unique_ptr<Expr> rhs)
      : Expr(sourceRange), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  BinExpr(LOC_ARG, const BinOp op, std::unique_ptr<Expr> &lhs,
          std::unique_ptr<Expr> &rhs)
      : Expr(sourceRange), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  virtual boost::json::value toJson() const override;
  static BinExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
