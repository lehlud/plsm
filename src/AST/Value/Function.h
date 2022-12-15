#pragma once

#include "../Type/Type.h"
#include "Value.h"

#include <map>
#include <vector>

namespace plsm {
namespace AST {

class Function : public Value {
public:
  typedef std::pair<std::string, std::shared_ptr<Type>> Arg;

  std::string name;

  std::vector<Arg> args;
  std::shared_ptr<Type> resultType;
  std::shared_ptr<Value> result;

  Function(std::vector<Arg> args, std::shared_ptr<Type> resultType,
           std::shared_ptr<Value> result)
      : args(args), resultType(resultType), result(result) {}

  llvm::Value *genCode(std::shared_ptr<Compiler::BuildContext> ctx) override;
};

} // namespace AST
} // namespace plsm
