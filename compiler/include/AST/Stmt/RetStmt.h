#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {
class RetStmt : public Stmt {
public:
  std::unique_ptr<Expr> value;

  RetStmt(LOC_ARG, std::unique_ptr<Expr> value)
      : Stmt(sourceRange), value(std::move(value)) {}

  virtual boost::json::value toJson() const override;
  static RetStmt *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
