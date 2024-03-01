#pragma once

#include "AST/Base.h"
#include <string>
#include <vector>
#include <memory>

namespace ast
{
  class GenericType : public Type
  {
    const std::string name;
    const std::vector<std::unique_ptr<Type>> types;

  public:
    GenericType(LOC_ARG, const std::string &name, std::vector<std::unique_ptr<Type>> &types)
        : Type(location), name(name), types(std::move(types)) {}
  };
}
