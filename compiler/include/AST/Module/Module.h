#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {
class Import;

class Module : public ASTNode {
public:
  const std::string name;
  std::vector<std::unique_ptr<Import>> imports;
  std::vector<std::unique_ptr<Stmt>> stmts;

  Module(LOC_ARG, const std::string &name,
         std::vector<std::unique_ptr<Import>> imports,
         std::vector<std::unique_ptr<Stmt>> stmts)
      : ASTNode(sourceRange), name(name), imports(std::move(imports)),
        stmts(std::move(stmts)) {}

  virtual boost::json::value toJson() const override;
  static Module *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
