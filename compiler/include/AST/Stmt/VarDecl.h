#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>

namespace plsm {
namespace ast {

class VarDecl : public Stmt {
  const std::string name;
  const std::shared_ptr<TypeName> typeName;

public:
  VarDecl(LOC_ARG, const std::string &name, TypeName *typeName)
      : Stmt(sourceRange), name(name), typeName(typeName) {}

  virtual boost::json::value toJson() override;
  static VarDecl *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
