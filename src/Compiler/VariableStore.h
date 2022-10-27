#pragma once

#include <memory>

#include <llvm/IR/Value.h>

namespace plsm
{
  namespace Compiler
  {
    class VariableStore
    {
    public:
          std::shared_ptr<llvm::Value *> llvmValue;
    };
  }
}