#include "AST/Def.h"
#include "AST/Type/Named.h"

namespace plsm {

boost::json::value ast::NamedType::toJson() {
  return {
      {"@type", "NamedType"},
      {"name", name},
  };
}

std::unique_ptr<ast::NamedType>
ast::NamedType::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::NamedType, std::string>(json, "name");
  return std::make_unique<ast::NamedType>(ast::Location::json(), name);
}

} // namespace plsm
