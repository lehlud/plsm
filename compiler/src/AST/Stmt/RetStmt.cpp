#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value RetStmt::toJson() const {
  return {
      {"@type", "RetStmt"},
      {"value", value->toJson()},
  };
}

RetStmt *RetStmt::fromJson(boost::json::value json) {
  auto value = fromJsonProperty<RetStmt, Expr>(json, "value");
  return new RetStmt(SourceRange::json(), std::move(value));
}

} // namespace ast
} // namespace plsm
