#pragma once

#include "AST/Base.h"
#include <string>

namespace plsm {
namespace ast {
class Identifier : public Expr {
  const std::string name;

public:
  Identifier(LOC_ARG, const std::string &name)
      : Expr(sourceRange), name(name) {}

  virtual boost::json::value toJson() override;
  static Identifier *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
