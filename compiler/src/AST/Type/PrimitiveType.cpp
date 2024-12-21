#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value PrimitiveType::toJson() {
  return {
      {"@type", "PrimitiveType"},
      {"name", name},
  };
}

PrimitiveType *PrimitiveType::fromJson(boost::json::value json) {
  auto name = getJsonValue<PrimitiveType, std::string>(json, "name");
  return new PrimitiveType(name);
}

} // namespace ast
} // namespace plsm