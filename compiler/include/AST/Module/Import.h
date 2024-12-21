#pragma once

#include "AST/Base.h"
#include <string>

namespace plsm {
namespace ast {
class Import : public ASTNode {
  const std::string moduleName;

public:
  Import(LOC_ARG, const std::string &moduleName)
      : ASTNode(sourceRange), moduleName(moduleName) {}

  virtual boost::json::value toJson() override;
  static Import *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
