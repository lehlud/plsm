#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value CastExpr::toJson() const {
  return {
      {"@type", "CastExpr"},
      {"value", value->toJson()},
      {"typeName", typeName->toJson()},
  };
}

CastExpr *CastExpr::fromJson(boost::json::value json) {
  auto value = fromJsonProperty<CastExpr, Expr>(json, "value");
  auto typeName = fromJsonProperty<CastExpr, TypeName>(json, "typeName");
  return new CastExpr(SourceRange::json(), std::move(value),
                      std::move(typeName));
}

} // namespace ast
} // namespace plsm
