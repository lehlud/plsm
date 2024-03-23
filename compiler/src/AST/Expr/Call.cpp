#include "AST/Expr/Call.h"
#include "AST/Def.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::CallExpr::toJson() {
  return {
      {"@type", "CallExpr"},
      {"callee", callee->toJson()},
      {"args", utils::mapToJson(args)},
  };
}

std::unique_ptr<ast::CallExpr>
ast::CallExpr::fromJson(boost::json::value json) {
  auto callee = fromJsonProperty<ast::CallExpr, ast::Expr>(json, "callee");
  auto args = fromJsonVector<ast::CallExpr, ast::Expr>(json, "args");
  return std::make_unique<ast::CallExpr>(ast::Location::json(), callee, args);
}

} // namespace plsm
