#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value CastExpr::toJson() {
  return {
      {"@type", "CastExpr"},
      {"value", value->toJson()},
      {"typeName", typeName->toJson()},
  };
}

CastExpr *CastExpr::fromJson(boost::json::value json) {
  auto value = fromJsonProperty<CastExpr, Expr>(json, "value");
  auto typeName = fromJsonProperty<CastExpr, TypeName>(json, "typeName");
  return new CastExpr(SourceRange::json(), value, typeName);
}

} // namespace ast
} // namespace plsm
