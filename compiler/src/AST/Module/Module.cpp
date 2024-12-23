#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value Module::toJson() const {
  return {
      {"@type", "Module"},
      {"name", name},
      {"imports", utils::mapToJson(imports)},
      {"stmts", utils::mapToJson(stmts)},
  };
}

Module *Module::fromJson(boost::json::value json) {
  auto name = getJsonValue<Module, std::string>(json, "name");
  auto imports = fromJsonVector<Module, Import>(json, "imports");
  auto stmts = fromJsonVector<Module, Stmt>(json, "stmts");

  return new Module(SourceRange::json(), name, std::move(imports),
                    std::move(stmts));
}

} // namespace ast
} // namespace plsm
