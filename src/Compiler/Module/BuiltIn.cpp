#include <Compiler/Module/BuiltIn.h>

#include <Compiler/BuildContext.h>

namespace plsm
{
  namespace Compiler
  {
    llvm::Module *BuiltIn::createBuiltInModule()
    {
      auto ctx = BuildContext();

      return ctx.llvmModule->get();
    }
  }
}
