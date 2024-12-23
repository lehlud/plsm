#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value AssignStmt::toJson() const {
  return {
      {"@type", "AssignStmt"},
      {"lval", lval->toJson()},
      {"rval", rval->toJson()},
  };
}

AssignStmt *AssignStmt::fromJson(boost::json::value json) {
  auto lval = fromJsonProperty<AssignStmt, Expr>(json, "lval");
  auto rval = fromJsonProperty<AssignStmt, Expr>(json, "rval");
  return new AssignStmt(SourceRange::json(), std::move(lval), std::move(rval));
}

} // namespace ast
} // namespace plsm
