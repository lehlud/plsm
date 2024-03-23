#include "AST/Def.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::ClosureType::toJson() {
  return {
      {"@type", "ClosureType"},
      {"from", utils::mapToJson(from)},
      {"to", to->toJson()},
  };
}

std::unique_ptr<ast::ClosureType>
ast::ClosureType::fromJson(boost::json::value json) {
  auto from = fromJsonVector<ast::ClosureType, ast::Type>(json, "from");
  auto to = fromJsonProperty<ast::ClosureType, ast::Type>(json, "to");
  return std::make_unique<ast::ClosureType>(ast::Location::json(), from, to);
}

} // namespace plsm
