#pragma once

#include "AST/Base.h"
#include <vector>
#include <memory>

namespace ast
{
  class ClosureType : public Type
  {
    const std::vector<std::unique_ptr<Type>> from;
    const std::unique_ptr<Type> to;

  public:
    ClosureType(LOC_ARG, std::vector<std::unique_ptr<Type>> &from, std::unique_ptr<Type> &to)
        : Type(location), from(std::move(from)), to(std::move(to)) {}
  };
}