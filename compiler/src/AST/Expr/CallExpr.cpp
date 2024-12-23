#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value CallExpr::toJson() const {
  return {
      {"@type", "CallExpr"},
      {"callee", callee->toJson()},
      {"args", utils::mapToJson(args)},
  };
}

CallExpr *CallExpr::fromJson(boost::json::value json) {
  auto callee = fromJsonProperty<CallExpr, Expr>(json, "callee");
  auto args = fromJsonVector<CallExpr, Expr>(json, "args");
  return new CallExpr(SourceRange::json(), std::move(callee), std::move(args));
}

} // namespace ast
} // namespace plsm
