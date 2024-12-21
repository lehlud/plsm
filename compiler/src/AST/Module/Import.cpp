#include "AST/AST.h"
#include <memory>

namespace plsm {
namespace ast {

boost::json::value Import::toJson() {
  return {{"@type", "Import"}, {"moduleName", moduleName}};
}
Import *Import::fromJson(boost::json::value json) {
  auto moduleName = getJsonValue<Import, std::string>(json, "moduleName");
  return new Import(SourceRange::json(), moduleName);
}

} // namespace ast
} // namespace plsm
