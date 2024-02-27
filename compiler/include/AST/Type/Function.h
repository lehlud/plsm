#pragma once

#include "AST/Base.h"
#include <vector>

namespace ast
{
  class FunctionType : public Type
  {
  public:
    FunctionType(const std::vector<Type *> &from, const Type *to)
        : from(std::move(from)), to(to) {}

    ~FunctionType()
    {
      for (auto &arg : from)
        delete arg;

      delete to;
    }

    const std::vector<Type *> from;
    const Type *to;
  };
}