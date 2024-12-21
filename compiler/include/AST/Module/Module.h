#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {
class Import;

class Module : public ASTNode {
  const std::string name;
  std::vector<std::shared_ptr<Import>> imports;
  std::vector<std::shared_ptr<Stmt>> stmts;

public:
  Module(LOC_ARG, const std::string &name, const std::vector<Import *> &imports,
         const std::vector<Stmt *> &stmts)
      : ASTNode(sourceRange), name(name), imports(), stmts() {
    for (auto &import : imports) {
      this->imports.push_back(std::shared_ptr<Import>(import));
    }

    for (auto &stmt : stmts) {
      this->stmts.push_back(std::shared_ptr<Stmt>(stmt));
    }
  }

  virtual boost::json::value toJson() override;
  static Module *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
