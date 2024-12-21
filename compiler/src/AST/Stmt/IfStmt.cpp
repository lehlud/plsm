#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value IfStmt::toJson() {
  return {
      {"@type", "IfStmt"},
      {"condition", condition->toJson()},
      {"ifBody", utils::mapToJson(ifBody)},
      {"elseBody", utils::mapToJson(elseBody)},
  };
}

IfStmt *IfStmt::fromJson(boost::json::value json) {
  auto condition = fromJsonProperty<IfStmt, Expr>(json, "condition");
  auto ifBody = fromJsonVector<IfStmt, Stmt>(json, "ifBody");
  auto elseBody = fromJsonVector<IfStmt, Stmt>(json, "elseBody");
  return new IfStmt(SourceRange::json(), condition, ifBody, elseBody);
}

} // namespace ast
} // namespace plsm
