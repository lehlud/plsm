#pragma once

#include "AST/Base.h"

namespace plsm {
namespace ast {

class CastExpr : public Expr {
public:
  std::unique_ptr<Expr> value;
  std::unique_ptr<TypeName> typeName;

  CastExpr(LOC_ARG, std::unique_ptr<Expr> value,
           std::unique_ptr<TypeName> typeName)
      : Expr(sourceRange), value(std::move(value)),
        typeName(std::move(typeName)) {}

  virtual boost::json::value toJson() const override;
  static CastExpr *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
