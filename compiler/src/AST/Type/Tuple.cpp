#include "AST/Def.h"
#include "AST/Type/Tuple.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::TupleType::toJson() {
  return {
      {"@type", "TupleType"},
      {"types", utils::mapToJson(types)},
  };
}

std::unique_ptr<ast::TupleType>
ast::TupleType::fromJson(boost::json::value json) {
  auto types = fromJsonVector<ast::TupleType, ast::Type>(json, "types");
  return std::make_unique<ast::TupleType>(ast::Location::json(), types);
}

} // namespace plsm
