#include "AST/Def.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::Closure::toJson() {
  return {
      {"@type", "Closure"},
      {"args", utils::mapToJson(args)},
      {"body", body->toJson()},
  };
}

std::unique_ptr<ast::Closure> ast::Closure::fromJson(boost::json::value json) {
  auto args = fromJsonVector<ast::Closure, ast::FnArg>(json, "args");
  auto body = fromJsonProperty<ast::Closure, ast::Expr>(json, "body");
  return std::make_unique<ast::Closure>(ast::Location::json(), args, body);
}

} // namespace plsm
