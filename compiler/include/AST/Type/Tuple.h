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
    TupleType(LOC_ARG, std::vector<std::unique_ptr<Type>> &types)
        : Type(location), types(std::move(types)) {}
  };
}