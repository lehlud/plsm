#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>
#include <vector>

namespace plsm {
namespace ast {

class FnParam : public ASTNode {
public:
  const std::string name;
  std::unique_ptr<TypeName> typeName;

  std::shared_ptr<Symbol> symbol;

  FnParam(LOC_ARG, const std::string &name, std::unique_ptr<TypeName> typeName)
      : ASTNode(sourceRange), name(name), typeName(std::move(typeName)) {}

  virtual boost::json::value toJson() const override;
  static FnParam *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

class FnDecl : public Stmt {
public:
  const std::string name;
  std::vector<std::unique_ptr<FnParam>> params;
  std::unique_ptr<TypeName> returnTypeName;
  std::unique_ptr<Block> body;

  std::shared_ptr<Symbol> symbol;

  FnDecl(LOC_ARG, const std::string &name,
         std::vector<std::unique_ptr<FnParam>> params,
         std::unique_ptr<TypeName> returnTypeName, std::unique_ptr<Block> body)
      : Stmt(sourceRange), name(name), params(std::move(params)),
        returnTypeName(std::move(returnTypeName)), body(std::move(body)) {}

  virtual boost::json::value toJson() const override;
  static FnDecl *fromJson(boost::json::value json);

  virtual bool alywasReturns() const override {
    throw std::runtime_error("should not call FnDecl::alywasReturns");
  }

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
