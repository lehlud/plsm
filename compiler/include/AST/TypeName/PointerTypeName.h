#pragma once

#include "AST/Base.h"

namespace plsm {
namespace ast {

class PointerTypeName : public TypeName {
public:
  const std::unique_ptr<TypeName> baseTypeName;

  PointerTypeName(LOC_ARG, std::unique_ptr<TypeName> baseTypeName)
      : TypeName(sourceRange), baseTypeName(std::move(baseTypeName)) {}

  virtual boost::json::value toJson() const override;
  static PointerTypeName *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm