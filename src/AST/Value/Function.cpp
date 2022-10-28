#include "Function.h"

#include "Value.h"
#include "../Type/Type.h"
#include "../../Compiler/BuildContext.h"

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

      // initialize new scope
      ctx->variableScopes.push_back(Compiler::VariableScope());

      for (uint64_t i = 0; i < fn->arg_size(); i++)
      {
        auto alloc = ctx->llvmBuilder->CreateAlloca(argTypes[i]);
        ctx->llvmBuilder->CreateStore(fn->getArg(i), alloc);
        ctx->variableScopes.back().values[this->args[i].first] = alloc;
      }

      auto result = this->result->genCode(ctx);
      ctx->llvmBuilder->CreateRet(result);

      // destroy created scope
      ctx->variableScopes.pop_back();

      ctx->llvmBuilder->SetInsertPoint(prevBlock);

      return fn;
    }
  }
}