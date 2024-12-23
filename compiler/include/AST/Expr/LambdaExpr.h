#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {
class FnParam;

class LambdaExpr : public Expr {
public:
  std::vector<std::unique_ptr<FnParam>> params;
  std::unique_ptr<TypeName> returnTypeName;
  std::unique_ptr<Block> body;

  LambdaExpr(LOC_ARG, std::vector<std::unique_ptr<FnParam>> params,
             std::unique_ptr<TypeName> returnTypeName,
             std::unique_ptr<Block> body)
      : Expr(sourceRange), params(std::move(params)),
        returnTypeName(std::move(returnTypeName)), body(std::move(body)) {}

  virtual boost::json::value toJson() const override;
  static LambdaExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
