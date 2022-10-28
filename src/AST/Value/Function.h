#pragma once

#include "../Type/Type.h"

#include <map>
#include <vector>

namespace plsm
{
  namespace AST
  {
    class Type;
    class Value;

    class Function : public Value
    {
    public:
      std::string name;

      std::vector<std::pair<std::string, std::shared_ptr<Type>>> args;
      std::shared_ptr<Type> resultType;
      std::shared_ptr<Value> result;

      llvm::Value *genCode(std::shared_ptr<Compiler::BuildContext> ctx) override;
    };
  }
}
