#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value FnParam::toJson() const {
  return {
      {"@type", "FnParam"},
      {"name", name},
      {"typeName", typeName->toJson()},
  };
}

FnParam *FnParam::fromJson(boost::json::value json) {
  auto name = getJsonValue<FnParam, std::string>(json, "name");
  auto typeName = fromJsonProperty<FnParam, TypeName>(json, "typeName");
  return new FnParam(SourceRange::json(), name, std::move(typeName));
}

boost::json::value FnDecl::toJson() const {
  return {
      {"@type", "FnDecl"},
      {"name", name},
      {"params", utils::mapToJson(params)},
      {"returnTypeName", returnTypeName->toJson()},
      {"body", body->toJson()},
  };
}

FnDecl *FnDecl::fromJson(boost::json::value json) {
  auto name = getJsonValue<FnDecl, std::string>(json, "name");
  auto params = fromJsonVector<FnDecl, FnParam>(json, "params");
  auto returnTypeName =
      fromJsonProperty<FnDecl, TypeName>(json, "returnTypeName");
  auto body = fromJsonProperty<FnDecl, Block>(json, "body");
  return new FnDecl(SourceRange::json(), name, std::move(params),
                    std::move(returnTypeName), std::move(body));
}

} // namespace ast
} // namespace plsm
