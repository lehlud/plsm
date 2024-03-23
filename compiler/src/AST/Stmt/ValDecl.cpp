#include "AST/Def.h"

namespace plsm {

boost::json::value ast::ValDecl::toJson() {
  return {
      {"@type", "ValDecl"},
      {"name", name},
      {"type", type->toJson()},
      {"value", value->toJson()},
  };
}

std::unique_ptr<ast::ValDecl> ast::ValDecl::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::ValDecl, std::string>(json, "name");
  auto type = fromJsonProperty<ast::ValDecl, ast::Type>(json, "type");
  auto value = fromJsonProperty<ast::ValDecl, ast::Expr>(json, "value");
  return std::make_unique<ast::ValDecl>(ast::Location::json(), name, type, value);
}

} // namespace plsm
