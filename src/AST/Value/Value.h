#pragma once

#include <llvm/IR/Value.h>

namespace plsm {
namespace Compiler {
class BuildContext;
}

namespace AST {
class Value {
public:
  virtual llvm::Value *genCode(std::shared_ptr<Compiler::BuildContext> ctx) = 0;
};

} // namespace AST
} // namespace plsm