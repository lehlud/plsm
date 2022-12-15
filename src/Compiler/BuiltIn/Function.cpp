#include <Compiler/BuiltIn/Function.h>

namespace plsm
{
  namespace Compiler
  {
    namespace BuiltIn
    {
      llvm::Value *AddFunction::call(std::shared_ptr<BuildContext> ctx, std::vector<llvm::Value *> args)
      {
        assert(args.size() == 2);

        auto a = args[0], b = args[1];

        assert(a->getType()->isIntegerTy() || a->getType()->isDoubleTy());
        assert(b->getType()->isIntegerTy() || b->getType()->isDoubleTy());

        if (a->getType()->isIntegerTy())
        {
          if (b->getType()->isIntegerTy())
          {
            // just add ints
          }
          else // b is double
          {
            // cast a to double and add
          }
        }
        else // a is double
        {

          if (b->getType()->isIntegerTy())
          {
            // cast b to double and add
          }
          else // b is double
          {
            // just add doubles
          }
        }
      }

      // TODO: log error and exit
    }
  }
}
