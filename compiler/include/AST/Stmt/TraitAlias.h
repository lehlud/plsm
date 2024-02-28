#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>
#include <memory>

namespace ast
{
  class TraitAlias : public Stmt
  {
    const std::string alias;
    const std::string aliased;

  public:
    TraitAlias(const std::string &alias, const std::string &aliased)
        : alias(alias), aliased(aliased) {}
  };
}