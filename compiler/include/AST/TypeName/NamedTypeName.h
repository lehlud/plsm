#pragma once

#include "AST/Base.h"

namespace plsm {
namespace ast {

class NamedTypeName : public TypeName {
public:
  const std::string name;

  NamedTypeName(LOC_ARG, const std::string &name)
      : TypeName(sourceRange), name(name) {}

  bool operator==(const NamedTypeName &other) { return name == other.name; }
  bool operator!=(const NamedTypeName &other) { return !(*this == other); }

  virtual boost::json::value toJson() const override;
  static NamedTypeName *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm