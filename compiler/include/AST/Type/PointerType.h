#pragma once

#include "AST/Base.h"

namespace plsm {
namespace ast {

class PointerType : public Type {
public:
  const std::shared_ptr<Type> baseType;

  PointerType(const std::shared_ptr<Type> &baseType) : Type(), baseType(baseType) {}

  virtual TypeName *toTypeName() override;

  virtual bool operator==(const Type &other) override {
    if (const PointerType *pt = dynamic_cast<const PointerType *>(&other)) {
      return *baseType == *pt->baseType;
    }

    return false;
  }

  virtual boost::json::value toJson() const override;
  static PointerType *fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
