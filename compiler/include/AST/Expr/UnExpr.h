#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>

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
  const std::shared_ptr<Expr> expr;

  UnExpr(LOC_ARG, const UnOp op, Expr *expr)
      : Expr(sourceRange), op(op), expr(expr) {}

  virtual boost::json::value toJson() override;
  static UnExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
