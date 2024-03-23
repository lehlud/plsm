#include "AST/Expr/BinExpr.h"
#include "AST/Base.h"
#include "AST/Def.h"
#include <memory>

namespace plsm {

boost::json::value ast::BinExpr::toJson() {
  return {
      {"@type", "BinExpr"},
      {"op", op},
      {"left", left->toJson()},
      {"right", right->toJson()},
  };
}

std::unique_ptr<ast::BinExpr> ast::BinExpr::fromJson(boost::json::value json) {
  auto op = getJsonValue<ast::BinExpr, std::string>(json, "op");
  auto left = fromJsonProperty<ast::BinExpr, ast::Expr>(json, "left");
  auto right = fromJsonProperty<ast::BinExpr, ast::Expr>(json, "right");
  return std::make_unique<ast::BinExpr>(ast::Location::json(), left, op, right);
}

boost::json::value ast::PrefExpr::toJson() {
  return {
      {"@type", "PrefExpr"},
      {"expr", expr->toJson()},
  };
}

std::unique_ptr<ast::PrefExpr>
ast::PrefExpr::fromJson(boost::json::value json) {
  auto expr = fromJsonProperty<ast::PrefExpr, ast::Expr>(json, "expr");
  return std::make_unique<ast::PrefExpr>(ast::Location::json(), expr);
}

} // namespace plsm
