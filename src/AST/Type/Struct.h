#pragma once

#include "Type.h"

#include <map>
#include <vector>

namespace plsm
{
  namespace AST
  {

    class Struct : public Type
    {
    public:
      std::map<std::string, std::shared_ptr<Type>> fields;

      llvm::Type *llvmType(std::shared_ptr<Compiler::BuildContext> ctx) override;
    };

  }
}