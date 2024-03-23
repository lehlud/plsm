#include "AST/Def.h"
#include <memory>

namespace plsm {

boost::json::value ast::PropExpr::toJson() {
  return {
      {"@type", "PropExpr"},
      {"expr", expr->toJson()},
      {"property", property},
  };
}

std::unique_ptr<ast::PropExpr>
ast::PropExpr::fromJson(boost::json::value json) {
  auto expr = fromJsonProperty<ast::PropExpr, ast::Expr>(json, "expr");
  auto property = getJsonValue<ast::PropExpr, std::string>(json, "property");
  return std::make_unique<ast::PropExpr>(ast::Location::json(), expr, property);
}

} // namespace plsm
