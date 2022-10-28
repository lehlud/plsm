#include "Number.h"

#include "../../Compiler/BuildContext.h"

#include <llvm/ADT/APFloat.h>
#include <llvm/IR/Constants.h>

namespace plsm
{
  namespace AST
  {
    llvm::Value *Number::genCode(std::shared_ptr<Compiler::BuildContext> ctx)
    {
      return llvm::ConstantFP::get(ctx->getNumTy(), this->value);
    }
  }
}