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
    TypeAlias(LOC_ARG, const std::string &alias, std::unique_ptr<Type> &type)
        : Stmt(location), alias(alias), type(std::move(type)) {}
  };
}