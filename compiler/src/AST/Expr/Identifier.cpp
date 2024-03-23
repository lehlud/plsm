#include "AST/Def.h"
#include "AST/Expr/Identifier.h"

namespace plsm {

boost::json::value ast::Identifier::toJson() {
  return {
      {"@type", "Identifier"},
      {"name", name},
  };
}

std::unique_ptr<ast::Identifier>
ast::Identifier::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::Identifier, std::string>(json, "name");
  return std::make_unique<ast::Identifier>(ast::Location::json(), name);
}

} // namespace plsm
