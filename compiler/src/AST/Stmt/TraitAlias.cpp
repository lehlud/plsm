#include "AST/Stmt/TraitAlias.h"
#include "AST/Def.h"

namespace plsm {

boost::json::value ast::TraitAlias::toJson() {
  return {
      {"@type", "TraitAlias"},
      {"alias", alias},
      {"aliased", aliased},
  };
}

std::unique_ptr<ast::TraitAlias>
ast::TraitAlias::fromJson(boost::json::value json) {
  auto alias = getJsonValue<ast::TraitAlias, std::string>(json, "alias");
  auto aliased = getJsonValue<ast::TraitAlias, std::string>(json, "aliased");
  return std::make_unique<ast::TraitAlias>(ast::Location::json(), alias,
                                           aliased);
}

} // namespace plsm
