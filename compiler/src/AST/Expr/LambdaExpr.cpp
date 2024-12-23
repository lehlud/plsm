#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value LambdaExpr::toJson() const {
  return {
      {"@type", "LambdaExpr"},
      {"params", utils::mapToJson(params)},
      {"returnTypeName", returnTypeName->toJson()},
      {"body", body->toJson()},
  };
}

LambdaExpr *LambdaExpr::fromJson(boost::json::value json) {
  auto params = fromJsonVector<LambdaExpr, FnParam>(json, "params");
  auto returnTypeName =
      fromJsonProperty<LambdaExpr, TypeName>(json, "returnTypeName");
  auto body = fromJsonProperty<LambdaExpr, Block>(json, "body");
  return new LambdaExpr(SourceRange::json(), std::move(params),
                        std::move(returnTypeName), std::move(body));
}

} // namespace ast
} // namespace plsm
