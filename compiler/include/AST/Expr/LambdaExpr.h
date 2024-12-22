#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>
#include <vector>

namespace plsm {
namespace ast {
class FnParam;

class LambdaExpr : public Expr {
public:
  std::vector<std::shared_ptr<FnParam>> params;
  std::vector<std::shared_ptr<Expr>> body;

  LambdaExpr(LOC_ARG, std::vector<FnParam *> params, std::vector<Expr *> body)
      : Expr(sourceRange), params(), body() {
    for (auto &param : params) {
      this->params.push_back(std::shared_ptr<FnParam>(param));
    }

    for (auto &expr : body) {
      this->body.push_back(std::shared_ptr<Expr>(expr));
    }
  }

  virtual boost::json::value toJson() override;
  static LambdaExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
