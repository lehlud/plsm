#include "AST/Expr/Block.h"
#include "AST/Base.h"
#include "AST/Def.h"
#include "Utils.h"
#include <algorithm>
#include <memory>

namespace plsm {

boost::json::value ast::BlockExpr::toJson() {
  return {
      {"@type", "BlockExpr"},
      {"stmts", utils::mapToJson(stmts)},
      {"result", result->toJson()},
  };
}

std::unique_ptr<ast::BlockExpr>
ast::BlockExpr::fromJson(boost::json::value json) {
  auto stmts = fromJsonVector<ast::BlockExpr, ast::Stmt>(json, "stmts");
  auto result = fromJsonProperty<ast::BlockExpr, ast::Expr>(json, "result");
  return std::make_unique<ast::BlockExpr>(ast::Location::json(), stmts, result);
}

} // namespace plsm
