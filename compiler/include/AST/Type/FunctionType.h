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

  FunctionType(std::vector<std::shared_ptr<Type>> paramTypes,
               std::shared_ptr<Type> returnType)
      : Type(), paramTypes(paramTypes), returnType(returnType) {}

  virtual TypeName *toTypeName() override;

  virtual bool operator==(const Type &other) override {
    if (const FunctionType *ft = dynamic_cast<const FunctionType *>(&other)) {
      Type &lhsType = *returnType;
      Type &rhsType = *ft->returnType;
      if (lhsType != rhsType)
        return false;

      if (paramTypes.size() != ft->paramTypes.size())
        return false;

      for (ssize_t i = 0; i < paramTypes.size(); i++) {
        Type &lhsType = *paramTypes[i];
        Type &rhsType = *ft->paramTypes[i];
        if (lhsType != rhsType)
          return false;
      }

      return true;
    }

    return false;
  }

  virtual boost::json::value toJson() const override;
  static FunctionType *fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
