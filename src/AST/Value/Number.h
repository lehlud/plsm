#pragma once

#include "../Value/Value.h"

namespace plsm {
namespace AST {

class Number : public Value {
public:
  double value;

  llvm::Value *genCode(std::shared_ptr<Compiler::BuildContext> ctx) override;
};

} // namespace AST
} // namespace plsm