#pragma once

#include <map>
#include <string>
#include <memory>

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

namespace plsm
{
  namespace Compiler
  {
    class BuildContext
    {
    public:
      std::map<std::string, llvm::Type *> namedTypes;

      std::unique_ptr<llvm::Module> llvmModule;
      std::unique_ptr<llvm::LLVMContext> llvmContext;
      std::unique_ptr<llvm::IRBuilder<>> llvmBuilder;

      inline llvm::Type *getPtrTy()
      {
        return llvm::Type::getInt8PtrTy(*llvmContext);
      }

      inline llvm::Type *getNumTy()
      {
        return llvm::Type::getDoubleTy(*llvmContext);
      }
    };
  }
}
