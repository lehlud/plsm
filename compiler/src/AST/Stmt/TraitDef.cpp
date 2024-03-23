#include "AST/Def.h"
#include "AST/Stmt/FnDef.h"
#include "AST/Stmt/TraitDef.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::TraitDef::toJson() {
  return {
      {"@type", "TraitDef"},
      {"name", name},
      {"traits", utils::mapToJson(traits)},
  };
}

std::unique_ptr<ast::TraitDef>
ast::TraitDef::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::TraitDef, std::string>(json, "name");
  auto traits = fromJsonVector<ast::TraitDef, ast::FnDef>(json, "traits");
  return std::make_unique<ast::TraitDef>(ast::Location::json(), name, traits);
}

} // namespace plsm
