#include "AST/Type/FunctionType.h"
#include "AST/AST.h"
#include "AST/Base.h"
#include "Utils.h"
#include <memory>

namespace plsm {
namespace ast {

boost::json::value FunctionType::toJson() const {
  return {
      {"@type", "FunctionType"},
      {"paramTypes", utils::mapToJson(paramTypes)},
      {"returnType", returnType->toJson()},
  };
}

FunctionType *FunctionType::fromJson(boost::json::value json) {
  auto paramTypesU = fromJsonVector<FunctionType, Type>(json, "paramTypes");
  auto returnTypeU = fromJsonProperty<FunctionType, Type>(json, "returnType");

  std::vector<std::shared_ptr<Type>> paramTypes;
  for (auto &paramType : paramTypesU)
    paramTypes.push_back(std::shared_ptr<Type>(paramType.release()));

  auto returnType = std::shared_ptr<Type>(returnTypeU.release());

  return new FunctionType(paramTypes, returnType);
}

TypeName *FunctionType::toTypeName() { return nullptr; }

} // namespace ast
} // namespace plsm