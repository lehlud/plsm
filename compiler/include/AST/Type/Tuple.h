#pragma once

#include "AST/Def.h"
#include <vector>

namespace ast
{
  class TupleType : public Type
  {
  public:
    TupleType(const std::vector<Type *> &types) : types(std::move(types)) {}

    ~TupleType()
    {
      for (auto &type : types)
        delete type;
    }

    const std::vector<Type *> types;
  };
}