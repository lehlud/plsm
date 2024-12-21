#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value LambdaExpr::toJson() {
  return {
      {"@type", "LambdaExpr"},
      {"params", utils::mapToJson(params)},
      {"body", utils::mapToJson(body)},
  };
}

LambdaExpr *LambdaExpr::fromJson(boost::json::value json) {
  auto params = fromJsonVector<LambdaExpr, FnParam>(json, "params");
  auto body = fromJsonVector<LambdaExpr, Expr>(json, "body");
  return new LambdaExpr(SourceRange::json(), params, body);
}

} // namespace ast
} // namespace plsm
