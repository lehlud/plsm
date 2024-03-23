#pragma once

#include "AST/Base.h"
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

namespace plsm {
namespace ast {
class UnaryExpr : public Expr {
  const std::string op;
  const std::unique_ptr<Expr> expr;

public:
  UnaryExpr(LOC_ARG, const std::string &op, std::unique_ptr<Expr> &expr)
      : Expr(location), op(op), expr(std::move(expr)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<UnaryExpr> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
