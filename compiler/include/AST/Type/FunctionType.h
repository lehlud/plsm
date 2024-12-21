#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {
class FunctionType : public Type {
  std::vector<std::unique_ptr<Type>> paramTypes;
  const std::unique_ptr<Type> returnType;

public:
  FunctionType(const std::vector<Type *> &paramTypes, Type *returnType)
      : Type(), returnType(returnType) {
    for (auto &paramType : paramTypes) {
      this->paramTypes.push_back(std::unique_ptr<Type>(paramType));
    }
  }

  virtual boost::json::value toJson() override;
  static FunctionType *fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
