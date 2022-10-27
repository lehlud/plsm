#include "Function.h"

#include "Type.h"
#include "Value.h"
#include "../Compiler/BuildContext.h"

#include <llvm/IR/Function.h>

namespace plsm
{
  namespace AST
  {
    llvm::Value *Function::genCode(std::shared_ptr<Compiler::BuildContext> ctx)
    {
      std::vector<llvm::Type *> argTypes;
      for (auto &arg : args)
      {
        argTypes.push_back(arg.second->llvmType(ctx));
      }

      auto fnType = llvm::FunctionType::get(resultType->llvmType(ctx), argTypes, false);
      auto fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, name, *ctx->llvmModule);

      auto prevBlock = ctx->llvmBuilder->GetInsertBlock();
      auto block = llvm::BasicBlock::Create(*ctx->llvmContext, "", fn);
      ctx->llvmBuilder->SetInsertPoint(block);

      // TODO: Allocate arguments

      auto result = this->result->genCode(ctx);
      ctx->llvmBuilder->CreateRet(result);

      ctx->llvmBuilder->SetInsertPoint(prevBlock);

      return fn;
    }
  }
}