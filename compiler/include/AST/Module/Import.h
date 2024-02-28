#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class Import : public ASTNode
  {
    const std::string moduleName;

  public:
    Import(const std::string &moduleName) : moduleName(moduleName) {}
  };
}