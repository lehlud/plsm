#include "AST/Def.h"
#include <memory>

namespace plsm {

boost::json::value ast::Import::toJson() {
  return {
      {"@type", "Import"},
      {"moduleName", moduleName}
  };
}

std::unique_ptr<ast::Import>
ast::Import::fromJson(boost::json::value json) {
  auto moduleName = getJsonValue<ast::Import, std::string>(json, "moduleName");
  return std::make_unique<ast::Import>(ast::Location::json(), moduleName);
}

} // namespace plsm
