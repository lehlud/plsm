#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {
class ExprStmt : public Stmt {
  const std::unique_ptr<Expr> expr;

public:
  ExprStmt(LOC_ARG, std::unique_ptr<Expr> &expr)
      : Stmt(location), expr(std::move(expr)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<ExprStmt> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
