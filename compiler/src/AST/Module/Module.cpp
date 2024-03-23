#include "AST/Def.h"
#include "Utils.h"

namespace plsm {

boost::json::value ast::Module::toJson() {
  return {
      {"@type", "Module"},
      {"name", name},
      {"imports", utils::mapToJson(imports)},
      {"stmts", utils::mapToJson(stmts)},
  };
}

std::unique_ptr<ast::Module> ast::Module::fromJson(boost::json::value json) {
  auto name = getJsonValue<ast::Module, std::string>(json, "name");
  auto imports = fromJsonVector<ast::Module, ast::Import>(json, "imports");
  auto stmts = fromJsonVector<ast::Module, ast::Stmt>(json, "stmts");
  return std::make_unique<ast::Module>(ast::Location::json(), name, imports, stmts);
}

} // namespace plsm
