#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value WhileStmt::toJson() const {
  return {
      {"@type", "WhileStmt"},
      {"condition", condition->toJson()},
      {"body", body->toJson()},
  };
}

WhileStmt *WhileStmt::fromJson(boost::json::value json) {
  auto condition = fromJsonProperty<WhileStmt, Expr>(json, "condition");
  auto body = fromJsonProperty<WhileStmt, Block>(json, "body");
  return new WhileStmt(SourceRange::json(), std::move(condition),
                       std::move(body));
}

} // namespace ast
} // namespace plsm
