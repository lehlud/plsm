#pragma once

#include "AST/Base.h"

namespace plsm {
namespace ast {

class PrimitiveTypeName : public TypeName {
  const std::string name;

public:
  PrimitiveTypeName(LOC_ARG, const std::string &name)
      : TypeName(sourceRange), name(name) {}

  bool operator==(const PrimitiveTypeName &other) { return name == other.name; }
  bool operator!=(const PrimitiveTypeName &other) { return !(*this == other); }

  virtual boost::json::value toJson() override;
  static PrimitiveTypeName *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm