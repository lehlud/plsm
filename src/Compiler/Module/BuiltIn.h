#pragma once

#include <llvm/IR/Module.h>

namespace plsm
{
  namespace Compiler
  {
    class BuiltIn
    {
    public:
      static llvm::Module *createBuiltInModule();
    };
  }
}