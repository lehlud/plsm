#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>

namespace plsm {
namespace ast {

class AssignStmt : public Stmt {
public:
  const std::shared_ptr<Expr> lval;
  const std::shared_ptr<Expr> rval;

  AssignStmt(LOC_ARG, Expr *lval, Expr *rval)
      : Stmt(sourceRange), lval(lval), rval(rval) {}

  virtual boost::json::value toJson() override;
  static AssignStmt *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
