#include "Type.h"

#include "../Compiler/BuildContext.h"

namespace plsm
{
  namespace AST
  {
    llvm::Type *TypeRef::llvmType(std::shared_ptr<Compiler::BuildContext> ctx)
    {
      return ctx->namedTypes[this->name];
    }
  }
}
