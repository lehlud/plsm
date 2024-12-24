#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {
class ExprStmt : public Stmt {
public:
  std::unique_ptr<Expr> expr;

  ExprStmt(LOC_ARG, std::unique_ptr<Expr> expr)
      : Stmt(sourceRange), expr(std::move(expr)) {}

  virtual boost::json::value toJson() const override;
  static ExprStmt *fromJson(boost::json::value json);

  virtual bool alywasReturns() const override { return false; }

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
