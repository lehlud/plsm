#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {

class AssignStmt : public Stmt {
public:
  std::unique_ptr<Expr> lval;
  std::unique_ptr<Expr> rval;

  AssignStmt(LOC_ARG, std::unique_ptr<Expr> lval, std::unique_ptr<Expr> rval)
      : Stmt(sourceRange), lval(std::move(lval)), rval(std::move(rval)) {}

  virtual boost::json::value toJson() const override;
  static AssignStmt *fromJson(boost::json::value json);

  virtual bool alywasReturns() const override { return false; }

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
