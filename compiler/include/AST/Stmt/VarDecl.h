#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>

namespace plsm {
namespace ast {

class VarDecl : public Stmt {
public:
  const std::string name;
  std::unique_ptr<TypeName> typeName;

  std::shared_ptr<Symbol> symbol;

  VarDecl(LOC_ARG, const std::string &name, std::unique_ptr<TypeName> typeName)
      : Stmt(sourceRange), name(name), typeName(std::move(typeName)) {}

  virtual boost::json::value toJson() const override;
  static VarDecl *fromJson(boost::json::value json);

  virtual bool alywasReturns() const override { return false; }

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
