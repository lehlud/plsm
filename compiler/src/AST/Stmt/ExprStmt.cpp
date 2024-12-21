#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value ExprStmt::toJson() {
  return {
      {"@type", "ExprStmt"},
      {"expr", expr->toJson()},
  };
}

ExprStmt *ExprStmt::fromJson(boost::json::value json) {
  auto expr = fromJsonProperty<ExprStmt, Expr>(json, "expr");
  return new ExprStmt(SourceRange::json(), expr);
}

} // namespace ast
} // namespace plsm
