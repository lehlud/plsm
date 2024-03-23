#pragma once

#include "AST/Base.h"
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

namespace plsm {
namespace ast {

class BinExpr : public Expr {
  const std::string op;
  const std::unique_ptr<Expr> left, right;

public:
  BinExpr(LOC_ARG, std::unique_ptr<Expr> &left, const std::string &op,
          std::unique_ptr<Expr> &right)
      : Expr(location), left(std::move(left)), op(op), right(std::move(right)) {
  }

  virtual boost::json::value toJson() override;
  static std::unique_ptr<BinExpr> fromJson(boost::json::value json);
};

class PrefExpr : public Expr {
  const std::unique_ptr<Expr> expr;

public:
  PrefExpr(LOC_ARG, std::unique_ptr<Expr> &expr)
      : Expr(location), expr(std::move(expr)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<PrefExpr> fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
