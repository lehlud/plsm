#pragma once

#include "AST/Base.h"
#include <string>

namespace plsm {
namespace ast {

class CastExpr : public Expr {
  const std::shared_ptr<Expr> value;
  const std::shared_ptr<TypeName> typeName;

public:
  CastExpr(LOC_ARG, Expr *value, TypeName *typeName)
      : Expr(sourceRange), value(value), typeName(typeName) {}

  virtual boost::json::value toJson() override;
  static CastExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
