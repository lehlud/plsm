#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {
class RetStmt : public Stmt {
  const std::shared_ptr<Expr> value;

public:
  RetStmt(LOC_ARG, Expr *value) : Stmt(sourceRange), value(value) {}

  virtual boost::json::value toJson() override;
  static RetStmt *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
