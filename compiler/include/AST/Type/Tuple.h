#pragma once

#include "AST/Def.h"
#include <vector>
#include <memory>

namespace ast
{
  class TupleType : public Type
  {
    const std::vector<std::unique_ptr<Type>> types;

  public:
    TupleType(std::vector<std::unique_ptr<Type>> &types)
        : types(std::move(types)) {}
  };
}