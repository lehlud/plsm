#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>
#include <memory>

namespace ast
{
  class TypeAlias : public Stmt
  {
    const std::string alias;
    const std::unique_ptr<Type> type;

  public:
    TypeAlias(const std::string &alias, std::unique_ptr<Type> &type)
        : alias(alias), type(std::move(type)) {}
  };
}