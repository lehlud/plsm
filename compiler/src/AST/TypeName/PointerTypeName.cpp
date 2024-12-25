#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value PointerTypeName::toJson() const {
  return {
      {"@type", "PointerTypeName"},
      {"baseTypeName", baseTypeName->toJson()},
  };
}

PointerTypeName *PointerTypeName::fromJson(boost::json::value json) {
  auto baseTypeName = fromJsonProperty<PointerTypeName, TypeName>(json, "baseTypeName");
  return new PointerTypeName(SourceRange::json(), std::move(baseTypeName));
}

} // namespace ast
} // namespace plsm