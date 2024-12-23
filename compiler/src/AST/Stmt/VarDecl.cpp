#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value VarDecl::toJson() const {
  return {
      {"@type", "VarDecl"},
      {"name", name},
      {"typeName", typeName->toJson()},
  };
}

VarDecl *VarDecl::fromJson(boost::json::value json) {
  auto name = getJsonValue<VarDecl, std::string>(json, "name");
  auto typeName = fromJsonProperty<VarDecl, TypeName>(json, "typeName");
  return new VarDecl(SourceRange::json(), name, std::move(typeName));
}

} // namespace ast
} // namespace plsm
