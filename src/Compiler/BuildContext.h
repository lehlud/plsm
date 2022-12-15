#pragma once

#include <map>
#include <string>
#include <memory>

#include "VariableScope.h"
#include "VariableStore.h"

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
      std::vector<VariableScope> variableScopes;
      std::map<std::string, llvm::Type *> namedTypes;

      std::unique_ptr<llvm::Module> llvmModule;
      std::unique_ptr<llvm::LLVMContext> llvmContext;
      std::unique_ptr<llvm::IRBuilder<>> llvmBuilder;

      inline VariableStore *getStore(std::string name)
      {
        // search for an existing store and update it (if the store exists)
        for (auto it = variableScopes.end(); it != variableScopes.begin(); --it)
        {
          auto scope = *it.base();
          if (scope.values.count(name))
          {
            return &(scope.values.at(name));
          }
        }

        return nullptr;
      }

      inline void createStore(std::string name, llvm::Value *value)
      {
        variableScopes.back().values[name] = value;
      }

      inline llvm::Type *getPtrTy()
      {
        return llvm::Type::getInt8PtrTy(*llvmContext);
      }

      inline llvm::Type *getIntTy()
      {
        return llvm::Type::getInt64Ty(*llvmContext);
      }

      inline llvm::Type *getFloatTy()
      {
        return llvm::Type::getFloatingPointTy(*llvmContext, llvm::APFloatBase::BFloat());
      }
    };
  }
}
