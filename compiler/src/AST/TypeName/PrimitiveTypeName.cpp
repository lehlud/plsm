#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value NamedTypeName::toJson() const {
  return {{"@type", "NamedTypeName"}, {"name", name}};
}

NamedTypeName *NamedTypeName::fromJson(boost::json::value json) {
  auto name = getJsonValue<NamedTypeName, std::string>(json, "name");
  return new NamedTypeName(SourceRange::json(), name);
}

} // namespace ast
} // namespace plsm