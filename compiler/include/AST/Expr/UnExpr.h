#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {
enum UnOp {
  NOT,
  POS,
  NEG,
};

class UnExpr : public Expr {
public:
  const UnOp op;
  std::unique_ptr<Expr> expr;

  UnExpr(LOC_ARG, const UnOp op, std::unique_ptr<Expr> expr)
      : Expr(sourceRange), op(op), expr(std::move(expr)) {}

  virtual boost::json::value toJson() const override;
  static UnExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
