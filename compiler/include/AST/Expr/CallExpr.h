#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {

class CallExpr : public Expr {
public:
  std::unique_ptr<Expr> callee;
  std::vector<std::unique_ptr<Expr>> args;

  CallExpr(LOC_ARG, std::unique_ptr<Expr> callee,
           std::vector<std::unique_ptr<Expr>> args)
      : Expr(sourceRange), callee(std::move(callee)), args(std::move(args)) {}

  virtual boost::json::value toJson() const override;
  static CallExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
