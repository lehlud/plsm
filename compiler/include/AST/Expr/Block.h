#pragma once

#include "AST/Base.h"
#include <vector>
#include <memory>

namespace ast
{
  class BlockExpr : public Expr
  {
    const std::vector<std::unique_ptr<Stmt>> stmts;

  public:
    BlockExpr(std::vector<std::unique_ptr<Stmt>> &stmts) : stmts(std::move(stmts)) {}
  };
}