#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value IfStmt::toJson() const {
  return {
      {"@type", "IfStmt"},
      {"condition", condition->toJson()},
      {"ifBody", ifBody->toJson()},
      {"elseBody", elseBody->toJson()},
  };
}

IfStmt *IfStmt::fromJson(boost::json::value json) {
  auto condition = fromJsonProperty<IfStmt, Expr>(json, "condition");
  auto ifBody = fromJsonProperty<IfStmt, Block>(json, "ifBody");
  auto elseBody = fromJsonProperty<IfStmt, Block>(json, "elseBody");
  return new IfStmt(SourceRange::json(), std::move(condition),
                    std::move(ifBody), std::move(elseBody));
}

} // namespace ast
} // namespace plsm
