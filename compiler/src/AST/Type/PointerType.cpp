#include "AST/AST.h"
#include "AST/Base.h"
#include "AST/TypeName/NamedTypeName.h"
#include <memory>

namespace plsm {
namespace ast {

boost::json::value PointerType::toJson() const {
  return {
      {"@type", "PointerType"},
      {"baseType", baseType->toJson()},
  };
}

PointerType *PointerType::fromJson(boost::json::value json) {
  auto baseType = fromJsonProperty<PointerType, Type>(json, "baseType");
  return new PointerType(std::shared_ptr<Type>(baseType.release()));
}

TypeName *PointerType::toTypeName() { return nullptr; }

} // namespace ast
} // namespace plsm