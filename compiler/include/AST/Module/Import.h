#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class Import : public ASTNode
  {
    const std::string moduleName;

  public:
    Import(LOC_ARG, const std::string &moduleName)
        : ASTNode(location), moduleName(moduleName) {}
  };
}