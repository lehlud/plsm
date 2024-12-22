#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {
class FunctionType : public Type {
public:
  std::vector<std::shared_ptr<Type>> paramTypes;
  const std::shared_ptr<Type> returnType;

  FunctionType(const std::vector<Type *> &paramTypes, Type *returnType)
      : Type(), returnType(returnType) {
    for (auto &paramType : paramTypes) {
      this->paramTypes.push_back(std::shared_ptr<Type>(paramType));
    }
  }

  virtual boost::json::value toJson() override;
  static FunctionType *fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
