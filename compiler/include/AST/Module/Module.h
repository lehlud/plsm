#pragma once

#include "AST/Base.h"
#include <vector>

namespace ast
{
  class Import;

  class Module : public ASTNode
  {
  public:
    Module(const std::vector<Import *> &imports, const std::vector<Stmt *> &stmts)
        : imports(std::move(imports)), stmts(std::move(stmts)) {}

    ~Module()
    {
      for (auto &import : imports)
        delete import;

      for (auto &stmt : stmts)
        delete stmt;
    }

    const std::vector<Import *> &imports;
    const std::vector<Stmt *> stmts;
  };
}