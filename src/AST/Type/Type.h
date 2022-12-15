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

    class TypeAlias : public Type
    {
    public:
      std::string alias;
      std::shared_ptr<Type> type;

      TypeAlias(std::string alias, std::shared_ptr<Type> type) : alias(alias), type(type) {}

      llvm::Type *llvmType(std::shared_ptr<Compiler::BuildContext> ctx) override;
    };

    class StructType : public Type
    {
    public:
      std::vector<std::shared_ptr<Type>> fields;
    };

    class TypeRef : public Type
    {
    public:
      std::string name;

      TypeRef(std::string name) : name(name) {}

      llvm::Type *llvmType(std::shared_ptr<Compiler::BuildContext> ctx) override;
    };
  }
}
