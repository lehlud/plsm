#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class Import : public ASTNode
  {
  public:
    Import(const std::string &moduleName) : moduleName(moduleName) {}
    const std::string moduleName;
  };
}