#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value FunctionType::toJson() {
  return {
      {"@type", "FunctionType"},
      {"paramTypes", utils::mapToJson(paramTypes)},
      {"returnType", returnType->toJson()},
  };
}

FunctionType *FunctionType::fromJson(boost::json::value json) {
  auto paramTypes = fromJsonVector<FunctionType, Type>(json, "paramTypes");
  auto returnType = fromJsonProperty<FunctionType, Type>(json, "returnType");
  return new FunctionType(paramTypes, returnType);
}

} // namespace ast
} // namespace plsm