#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value PrimitiveTypeName::toJson() {
  return {{"@type", "PrimitiveTypeName"}, {"name", name}};
}

PrimitiveTypeName *PrimitiveTypeName::fromJson(boost::json::value json) {
  auto name = getJsonValue<PrimitiveTypeName, std::string>(json, "name");
  return new PrimitiveTypeName(SourceRange::json(), name);
}

} // namespace ast
} // namespace plsm