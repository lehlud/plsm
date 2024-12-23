#include "AST/AST.h"

namespace plsm {
namespace ast {

boost::json::value Identifier::toJson() const {
  return {
      {"@type", "Identifier"},
      {"name", name},
  };
}

Identifier *Identifier::fromJson(boost::json::value json) {
  auto name = getJsonValue<Identifier, std::string>(json, "name");
  return new Identifier(SourceRange::json(), name);
}

} // namespace ast
} // namespace plsm
