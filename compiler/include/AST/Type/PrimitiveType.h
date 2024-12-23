#pragma once

#include "AST/Base.h"

namespace plsm {
namespace ast {

class PrimitiveType : public Type {
public:
  const std::string name;

  PrimitiveType(const std::string &name) : Type(), name(name) {}

  virtual TypeName *toTypeName() override;

  virtual bool operator==(const Type &other) override {
    if (const PrimitiveType *pt = dynamic_cast<const PrimitiveType *>(&other)) {
      return name == pt->name;
    }

    return false;
  }

  virtual boost::json::value toJson() const override;
  static PrimitiveType *fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
