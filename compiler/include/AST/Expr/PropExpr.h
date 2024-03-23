#pragma once

#include "AST/Base.h"
#include <memory>
#include <sstream>
#include <string>

namespace plsm {
namespace ast {
class PropExpr : public Expr {
  const std::unique_ptr<Expr> expr;
  const std::string property;

public:
  PropExpr(LOC_ARG, std::unique_ptr<Expr> &expr, const std::string &property)
      : Expr(location), expr(std::move(expr)), property(property) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<PropExpr> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
