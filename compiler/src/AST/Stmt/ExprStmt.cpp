#include "AST/Stmt/ExprStmt.h"
#include "AST/Def.h"

namespace plsm {

boost::json::value ast::ExprStmt::toJson() {
  return {{"@type", "ExprStmt"}, {"expr", expr->toJson()}};
}

std::unique_ptr<ast::ExprStmt>
ast::ExprStmt::fromJson(boost::json::value json) {
  auto expr = fromJsonProperty<ast::ExprStmt, ast::Expr>(json, "expr");
  return std::make_unique<ast::ExprStmt>(ast::Location::json(), expr);
}

} // namespace plsm
