#include "AST/Def.h"
#include "AST/Type/Generic.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::GenericType::toJson() {
  return {
      {"@type", "GenericType"},
      {"name", name},
      {"types", utils::mapToJson(types)},
  };
}

std::unique_ptr<ast::GenericType>
ast::GenericType::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::GenericType, std::string>(json, "name");
  auto types = fromJsonVector<ast::GenericType, ast::Type>(json, "types");
  return std::make_unique<ast::GenericType>(ast::Location::json(), name, types);
}

} // namespace plsm
