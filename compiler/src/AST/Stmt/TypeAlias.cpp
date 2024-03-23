#include "AST/Def.h"

namespace plsm {

boost::json::value ast::TypeAlias::toJson() {
  return {
      {"@type", "TypeAlias"},
      {"alias", alias},
      {"type", type->toJson()},
  };
}

std::unique_ptr<ast::TypeAlias>
ast::TypeAlias::fromJson(boost::json::value json) {
  return nullptr;
}

} // namespace plsm
