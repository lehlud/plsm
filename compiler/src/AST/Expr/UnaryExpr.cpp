#include "AST/Def.h"
#include "AST/Expr/UnaryExpr.h"
#include <memory>

namespace plsm {

boost::json::value ast::UnaryExpr::toJson() {
  return {
      {"@type", "UnaryExpr"},
      {"op", op},
      {"expr", expr->toJson()},
  };
}

std::unique_ptr<ast::UnaryExpr>
ast::UnaryExpr::fromJson(boost::json::value json) {
  auto op = getJsonValue<ast::UnaryExpr, std::string>(json, "op");
  auto expr = fromJsonProperty<ast::UnaryExpr, ast::Expr>(json, "expr");
  return std::make_unique<ast::UnaryExpr>(ast::Location::json(), op, expr);
}

} // namespace plsm
