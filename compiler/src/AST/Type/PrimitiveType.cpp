#include "AST/AST.h"
#include "AST/Base.h"
#include "AST/TypeName/NamedTypeName.h"
#include <memory>

namespace plsm {
namespace ast {

boost::json::value PrimitiveType::toJson() const {
  return {
      {"@type", "PrimitiveType"},
      {"name", name},
  };
}

PrimitiveType *PrimitiveType::fromJson(boost::json::value json) {
  auto name = getJsonValue<PrimitiveType, std::string>(json, "name");
  return new PrimitiveType(name);
}

TypeName *PrimitiveType::toTypeName() {
  return new NamedTypeName(SourceRange::unknown(), name);
}

} // namespace ast
} // namespace plsm