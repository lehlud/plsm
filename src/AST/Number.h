#pragma once

#include "Value.h"

namespace plsm
{
  namespace AST
  {

    class Number : public Value
    {
    public:
      double value;

      llvm::Value *genCode(std::shared_ptr<Compiler::BuildContext> ctx) override;
    };

  }
}