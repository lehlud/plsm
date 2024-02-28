#pragma once

#include "AST/Base.h"
#include <vector>
#include <memory>

namespace ast
{
  class Import;

  class Module : public ASTNode
  {
    const std::vector<std::unique_ptr<Import>> imports;
    const std::vector<std::unique_ptr<Stmt>> stmts;

  public:
    Module(std::vector<std::unique_ptr<Import>> &imports, std::vector<std::unique_ptr<Stmt>> &stmts)
        : imports(std::move(imports)), stmts(std::move(stmts)) {}
  };
}