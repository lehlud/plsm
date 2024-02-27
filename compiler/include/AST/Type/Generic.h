#pragma once

#include "AST/Base.h"
#include <string>
#include <vector>

namespace ast
{
  class GenericType : public Type
  {
  public:
    GenericType(const std::string &name, const std::vector<Type *> &types)
        : name(name), types(std::move(types)) {}

    ~GenericType()
    {
      for (auto &type : types)
        delete type;
    }

    const std::string name;
    const std::vector<Type *> types;
  };
}
