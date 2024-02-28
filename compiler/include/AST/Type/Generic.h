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
    GenericType(const std::string &name, std::vector<std::unique_ptr<Type>> &types)
        : name(name), types(std::move(types)) {}
  };
}
