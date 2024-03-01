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
    TraitAlias(LOC_ARG, const std::string &alias, const std::string &aliased)
        : Stmt(location), alias(alias), aliased(aliased) {}
  };
}