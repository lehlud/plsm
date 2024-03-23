#include "AST/Def.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::Tuple::toJson() {
  return {
      {"@type", "Tuple"},
      {"values", utils::mapToJson(values)},
  };
}

std::unique_ptr<ast::Tuple> ast::Tuple::fromJson(boost::json::value json) {
  auto values = fromJsonVector<ast::Tuple, ast::Expr>(json, "values");
  return std::make_unique<ast::Tuple>(ast::Location::json(), values);
}

} // namespace plsm
