#pragma once

#include "AST/Base.h"
#include <vector>

namespace ast
{
  class BlockExpr : public Expr
  {
  public:
    BlockExpr(const std::vector<Stmt *> &stmts) : stmts(std::move(stmts)) {}
    ~BlockExpr()
    {
      for (auto &stmt : stmts)
        delete stmt;
    }

    const std::vector<Stmt *> stmts;
  };
}