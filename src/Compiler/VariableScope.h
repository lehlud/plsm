#pragma once

#include <map>
#include <string>

#include <llvm/IR/Value.h>

namespace plsm
{
  namespace AST
  {
    class Value;
  }

  namespace Compiler
  {

    class VariableScope
    {
    public:
      std::map<std::string, llvm::Value *> values;
    };
  }
}
