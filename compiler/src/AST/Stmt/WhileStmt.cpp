#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value WhileStmt::toJson() {
  return {
      {"@type", "WhileStmt"},
      {"condition", condition->toJson()},
      {"body", utils::mapToJson(body)},
  };
}

WhileStmt *WhileStmt::fromJson(boost::json::value json) {
  auto condition = fromJsonProperty<WhileStmt, Expr>(json, "condition");
  auto body = fromJsonVector<WhileStmt, Stmt>(json, "body");
  return new WhileStmt(SourceRange::json(), condition, body);
}

} // namespace ast
} // namespace plsm
