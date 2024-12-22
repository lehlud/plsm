#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {
class ExprStmt : public Stmt {
public:
  const std::shared_ptr<Expr> expr;

  ExprStmt(LOC_ARG, Expr *expr) : Stmt(sourceRange), expr(expr) {}

  virtual boost::json::value toJson() override;
  static ExprStmt *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
