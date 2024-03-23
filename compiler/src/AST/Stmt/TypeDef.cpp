#include "AST/Def.h"
#include "AST/Stmt/FnDef.h"
#include "AST/Stmt/TypeDef.h"
#include "Utils.h"
#include <memory>

namespace plsm {

boost::json::value ast::TypeDef::toJson() {
  return {
      {"@type", "TypeDef"},
      {"name", name},
      {"attrs", utils::mapToJson(attrs)},
      {"members", utils::mapToJson(members)},
  };
}

std::unique_ptr<ast::TypeDef> ast::TypeDef::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::TypeDef, std::string>(json, "name");
  auto attrs = fromJsonVector<ast::TypeDef, ast::FnArg>(json, "attrs");
  auto members = fromJsonVector<ast::TypeDef, ast::FnDef>(json, "members");
  return std::make_unique<ast::TypeDef>(ast::Location::json(), name, attrs, members);
}

} // namespace plsm
