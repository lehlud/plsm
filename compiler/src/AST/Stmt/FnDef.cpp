#include "AST/Def.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::FnArg::toJson() {
  return {
      {"@type", "FnArg"},
      {"name", name},
      {"type", type->toJson()},
  };
}

std::unique_ptr<ast::FnArg> ast::FnArg::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::FnArg, std::string>(json, "name");
  auto type = fromJsonProperty<ast::FnArg, ast::Type>(json, "type");
  return std::make_unique<ast::FnArg>(ast::Location::json(), name, type);
}

boost::json::value ast::FnDef::toJson() {
  return {
      {"@type", "FnDef"},
      {"name", name},
      {"args", utils::mapToJson(args)},
      {"returnType", returnType->toJson()},
      {"body", body->toJson()},
  };
}

std::unique_ptr<ast::FnDef> ast::FnDef::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::FnDef, std::string>(json, "name");
  auto args = fromJsonVector<ast::FnDef, ast::FnArg>(json, "args");
  auto returnType = fromJsonProperty<ast::FnDef, ast::Type>(json, "returnType");
  auto body = fromJsonProperty<ast::FnDef, ast::Expr>(json, "body");
  return std::make_unique<ast::FnDef>(ast::Location::json(), name, args,
                                      returnType, body);
}

} // namespace plsm
