#pragma once

#include <vector>
#include <memory>

#include <llvm/IR/Value.h>

#include <Compiler/BuildContext.h>

namespace plsm
{
  namespace Compiler
  {
    namespace BuiltIn
    {
      class Function
      {
      public:
        virtual llvm::Value *call(std::shared_ptr<BuildContext> ctx, std::vector<llvm::Value *> args) = 0;
      };

      class AddFunction : public Function
      {
      public:
        llvm::Value *call(std::shared_ptr<BuildContext> ctx, std::vector<llvm::Value *> args) override;
      };
    }
  }
}
