#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>
#include <vector>

namespace plsm {
namespace ast {

class FnParam : public ASTNode {
  const std::string name;
  const std::shared_ptr<TypeName> typeName;

public:
  FnParam(LOC_ARG, const std::string &name, TypeName *typeName)
      : ASTNode(sourceRange), name(name), typeName(typeName) {}

  virtual boost::json::value toJson() override;
  static FnParam *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

class FnDecl : public Stmt {
  const std::string name;
  std::vector<std::shared_ptr<FnParam>> params;
  const std::shared_ptr<TypeName> returnTypeName;
  std::vector<std::shared_ptr<Stmt>> body;

public:
  FnDecl(LOC_ARG, const std::string &name, const std::vector<FnParam *> &params,
         TypeName *returnTypeName, const std::vector<Stmt *> &body)
      : Stmt(sourceRange), name(name), params(), returnTypeName(returnTypeName),
        body() {
    for (auto &param : params) {
      this->params.push_back(std::shared_ptr<FnParam>(param));
    }

    for (auto &stmt : body) {
      this->body.push_back(std::shared_ptr<Stmt>(stmt));
    }
  }

  virtual boost::json::value toJson() override;
  static FnDecl *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
