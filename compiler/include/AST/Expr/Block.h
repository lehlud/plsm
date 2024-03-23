#pragma once

#include "AST/Base.h"
#include <memory>
#include <sstream>
#include <vector>

namespace plsm {
namespace ast {

class BlockExpr : public Expr {
  const std::vector<std::unique_ptr<Stmt>> stmts;
  const std::unique_ptr<Expr> result;

public:
  BlockExpr(LOC_ARG, std::vector<std::unique_ptr<Stmt>> &stmts,
            std::unique_ptr<Expr> &result)
      : Expr(location), stmts(std::move(stmts)), result(std::move(result)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<BlockExpr> fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
