#pragma once

#include <string>
#include <memory>

#include <llvm/IR/Type.h>

namespace plsm
{
  namespace Compiler
  {
    class BuildContext;
  }

  namespace AST
  {
    class Type
    {
    public:
      virtual llvm::Type *llvmType(std::shared_ptr<Compiler::BuildContext> ctx) = 0;
    };

    class TypeRef : public Type
    {
    public:
      std::string name;

      virtual llvm::Type *llvmType(std::shared_ptr<Compiler::BuildContext> ctx) override;
    };
  }
}
