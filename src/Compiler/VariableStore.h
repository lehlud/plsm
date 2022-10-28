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
      llvm::Value *llvmValue;

      VariableStore(llvm::Value *llvmValue) : llvmValue(llvmValue) {}
    };
  }
}