#pragma once

#include "AST/Base.h"
#include <vector>
#include <memory>

namespace ast
{
  class BlockExpr : public Expr
  {
    const std::vector<std::unique_ptr<Stmt>> stmts;
    const std::unique_ptr<Expr> result;

  public:
    BlockExpr(LOC_ARG, std::vector<std::unique_ptr<Stmt>> &stmts, std::unique_ptr<Expr> &result)
        : Expr(location), stmts(std::move(stmts)), result(std::move(result)) {}

    virtual std::string str()
    {
      std::stringstream ss;
      ss << "BlockExpr(";

      ss << "stmts=[";
      for (size_t i = 0; i < stmts.size(); i++)
      {
        ss << stmts[i]->str();
        if (i != stmts.size() - 1)
          ss << ", ";
      }
      ss << "], ";

      ss << "result=" << result->str();
      ss << ")";

      return ss.str();
    }
  };
}