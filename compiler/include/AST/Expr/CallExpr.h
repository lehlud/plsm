#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {

class CallExpr : public Expr {
public:
  const std::shared_ptr<Expr> callee;
  std::vector<std::shared_ptr<Expr>> args;

  CallExpr(LOC_ARG, Expr *callee, std::vector<Expr *> args)
      : Expr(sourceRange), callee(callee), args() {
    for (auto &arg : args) {
      this->args.push_back(std::shared_ptr<Expr>(arg));
    }
  }

  virtual boost::json::value toJson() override;
  static CallExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
