#include "AST/BaseASTVisitor.h"
#include "Compile.h"
#include "Utils.h"

#include <map>
#include <vector>

#include <llvm/ADT/Hashing.h>
#include <llvm/CodeGen/MachineModuleInfo.h>
#include <llvm/CodeGen/Passes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

namespace plsm {
using namespace ast;

namespace {

static llvm::Type *getLLVMType(llvm::LLVMContext &ctx,
                               const std::shared_ptr<Type> &type) {
  if (utils::is<PrimitiveType>(type.get())) {
    auto primitiveType = (PrimitiveType *)type.get();
    if (primitiveType->name == "i8" || primitiveType->name == "u8")
      return llvm::Type::getInt8Ty(ctx);
    if (primitiveType->name == "i16" || primitiveType->name == "u16")
      return llvm::Type::getInt16Ty(ctx);
    if (primitiveType->name == "i32" || primitiveType->name == "u32")
      return llvm::Type::getInt32Ty(ctx);
    if (primitiveType->name == "i64" || primitiveType->name == "u64")
      return llvm::Type::getInt64Ty(ctx);
    if (primitiveType->name == "i128" || primitiveType->name == "u128")
      return llvm::Type::getInt128Ty(ctx);
    if (primitiveType->name == "float")
      return llvm::Type::getFloatTy(ctx);
    if (primitiveType->name == "double")
      return llvm::Type::getDoubleTy(ctx);
  }

  else if (utils::is<FunctionType>(type.get())) {
    auto functionType = (FunctionType *)type.get();
    auto returnType = getLLVMType(ctx, functionType->returnType);

    std::vector<llvm::Type *> llvmParams;
    llvmParams.push_back(llvm::IntegerType::get(ctx, 8)->getPointerTo());
    for (auto &paramType : functionType->paramTypes)
      llvmParams.push_back(getLLVMType(ctx, paramType));

    return llvm::FunctionType::get(returnType, llvmParams, false);
  }

  throw std::runtime_error("cannot determine llvm type");
  return nullptr;
}
class IRGenerator1 : public BaseASTVisitor {
  llvm::LLVMContext &ctx;
  llvm::Module &mod;
  llvm::IRBuilder<> &builder;

  std::map<Symbol *, llvm::Value *> &symbolMap;
  std::set<llvm::Value *> &functions;

public:
  IRGenerator1(llvm::LLVMContext &ctx, llvm::Module &mod,
               llvm::IRBuilder<> &builder,
               std::map<Symbol *, llvm::Value *> &symbolMap,
               std::set<llvm::Value *> &functions)
      : ctx(ctx), mod(mod), builder(builder), symbolMap(symbolMap),
        functions(functions) {}

  virtual std::any visit(VarDecl &varDecl, std::any param) override {
    auto llvmType = getLLVMType(ctx, varDecl.symbol->type);

    // auto global = new llvm::GlobalVariable(mod, llvmType, false,
    //                                        llvm::GlobalValue::ExternalLinkage,
    //                                        nullptr, varDecl.name);
    auto global = mod.getOrInsertGlobal(varDecl.name, llvmType);

    symbolMap[varDecl.symbol.get()] = global;

    return std::any();
  }

  virtual std::any visit(FnDecl &fnDecl, std::any param) override {
    auto functionType =
        (llvm::FunctionType *)getLLVMType(ctx, fnDecl.symbol->type);

    mod.getOrInsertFunction(fnDecl.name, functionType);
    auto function = mod.getFunction(fnDecl.name);
    symbolMap[fnDecl.symbol.get()] = function;
    functions.insert(function);

    return std::any();
  }
};

class IRGenerator2 : public BaseASTVisitor {
  llvm::LLVMContext &ctx;
  llvm::Module &mod;
  llvm::IRBuilder<> &builder;

  std::map<Symbol *, llvm::Value *> &symbolMap;
  std::set<llvm::Value *> &functions;
  std::set<llvm::Value *> lambdas;

  llvm::Value *retStore = nullptr;
  llvm::BasicBlock *retBlock = nullptr;

  llvm::Value *rvalueForLValue = nullptr;

  size_t labelCounter = 0;
  std::string createLabel() { return "L" + std::to_string(labelCounter++); }

  llvm::Value *wrapCallee(llvm::Value *callee) {
    auto ptr = llvm::IntegerType::get(ctx, 8)->getPointerTo();
    auto structType = llvm::StructType::get(ptr, callee->getType());

    auto store = builder.CreateAlloca(structType);
    auto ep = builder.CreateStructGEP(structType, store, 0);
    builder.CreateStore(llvm::ConstantPointerNull::get(ptr), ep);
    ep = builder.CreateStructGEP(structType, store, 1);
    builder.CreateStore(callee, ep);

    return store;

    // (f() -> asdf) -> { context, f() -> asdf }
  }

public:
  IRGenerator2(llvm::LLVMContext &ctx, llvm::Module &mod,
               llvm::IRBuilder<> &builder,
               std::map<Symbol *, llvm::Value *> &symbolMap,
               std::set<llvm::Value *> &functions)
      : ctx(ctx), mod(mod), builder(builder), symbolMap(symbolMap),
        functions(functions) {}

  virtual std::any visit(FnDecl &fnDecl, std::any param) override {
    auto function = mod.getFunction(fnDecl.name);
    auto block = llvm::BasicBlock::Create(ctx, createLabel(), function);
    builder.SetInsertPoint(block);

    size_t i = 0;
    for (auto &arg : function->args()) {
      if (i > 0) {
        auto store = builder.CreateAlloca(arg.getType(), nullptr);
        builder.CreateStore(&arg, store);
        symbolMap[fnDecl.params[i - 1]->symbol.get()] = store;
      }

      i += 1;
    }

    auto fnType = (llvm::FunctionType *)getLLVMType(ctx, fnDecl.symbol->type);
    retStore = builder.CreateAlloca(fnType->getReturnType());
    retBlock = llvm::BasicBlock::Create(ctx, createLabel(), function);

    BaseASTVisitor::visit(fnDecl, param);
    if (!fnDecl.body->alywasReturns())
      builder.CreateBr(retBlock);

    builder.SetInsertPoint(retBlock);
    auto retVal = builder.CreateLoad(fnType->getReturnType(), retStore);
    builder.CreateRet(retVal);

    return std::any();
  }

  virtual std::any visit(VarDecl &varDecl, std::any param) override {
    auto store = builder.CreateAlloca(getLLVMType(ctx, varDecl.symbol->type));
    symbolMap[varDecl.symbol.get()] = store;
    return std::any();
  }

  virtual std::any visit(BinExpr &binExpr, std::any param) override {
    auto lhs = std::any_cast<llvm::Value *>(binExpr.lhs->accept(this, param));
    auto rhs = std::any_cast<llvm::Value *>(binExpr.rhs->accept(this, param));

    auto primitiveType = (PrimitiveType *)binExpr.type.get();
    auto name = primitiveType->name;

    auto isFloat = name == "float" || name == "double";
    auto isUnsigned = name[0] == 'u';

    switch (binExpr.op) {
    case BinOp::ADD:
      if (isFloat)
        return builder.CreateFAdd(lhs, rhs);
      return builder.CreateAdd(lhs, rhs);
    case BinOp::SUB:
      if (isFloat)
        return builder.CreateFSub(lhs, rhs);
      return builder.CreateSub(lhs, rhs);
    case BinOp::MUL:
      if (isFloat)
        return builder.CreateFMul(lhs, rhs);
      return builder.CreateMul(lhs, rhs);
    case BinOp::DIV:
      if (isFloat)
        return builder.CreateFDiv(lhs, rhs);
      if (isUnsigned)
        return builder.CreateUDiv(lhs, rhs);
      return builder.CreateSDiv(lhs, rhs);
    case BinOp::MOD:
      if (isUnsigned)
        return builder.CreateURem(lhs, rhs);
      return builder.CreateSRem(lhs, rhs);
    case BinOp::EQ:
      return builder.CreateICmpEQ(lhs, rhs);
    case BinOp::NE:
      return builder.CreateICmpNE(lhs, rhs);
    case BinOp::LT:
      if (isFloat)
        return builder.CreateFCmpOGT(lhs, rhs);
      if (isUnsigned)
        return builder.CreateICmpULT(lhs, rhs);
      return builder.CreateICmpSLT(lhs, rhs);
    case BinOp::GT:
      if (isFloat)
        return builder.CreateFCmpOGT(lhs, rhs);
      if (isUnsigned)
        return builder.CreateICmpUGT(lhs, rhs);
      return builder.CreateICmpSGT(lhs, rhs);
    case BinOp::LE:
      if (isFloat)
        return builder.CreateFCmpOLE(lhs, rhs);
      if (isUnsigned)
        return builder.CreateICmpULE(lhs, rhs);
      return builder.CreateICmpSLE(lhs, rhs);
    case BinOp::GE:
      if (isFloat)
        return builder.CreateFCmpOGE(lhs, rhs);
      if (isUnsigned)
        return builder.CreateICmpUGE(lhs, rhs);
      return builder.CreateICmpSGE(lhs, rhs);
    case BinOp::AND:
      return builder.CreateAnd(lhs, rhs);
    case BinOp::OR:
      return builder.CreateOr(lhs, rhs);
    }

    throw std::runtime_error("binop not implemented");
  }

  virtual std::any visit(UnExpr &unExpr, std::any param) override {
    auto expr = std::any_cast<llvm::Value *>(unExpr.expr->accept(this, param));

    switch (unExpr.op) {
    case UnOp::NEG:
      return builder.CreateNeg(expr);
    case UnOp::NOT:
      return builder.CreateNot(expr);
    case UnOp::POS:
      return expr;
    }

    throw std::runtime_error("unop not implemented");
  }

  virtual std::any visit(CastExpr &castExpr, std::any param) override {
    auto value =
        std::any_cast<llvm::Value *>(castExpr.value->accept(this, param));

    if (utils::is<PrimitiveType>(castExpr.value->type.get()) &&
        utils::is<PrimitiveType>(castExpr.type.get())) {
      auto primitiveType = (PrimitiveType *)castExpr.value->type.get();
      auto newPrimitiveType = (PrimitiveType *)castExpr.type.get();

      auto newType = getLLVMType(ctx, castExpr.type);

      auto wasFloat =
          primitiveType->name == "float" || primitiveType->name == "double";
      auto wasUnsigned = primitiveType->name[0] == 'u';
      auto willFloat = newPrimitiveType->name == "float" ||
                       newPrimitiveType->name == "double";
      auto willUnsigned = newPrimitiveType->name[0] == 'u';

      if (wasFloat) {
        if (willFloat) {
          if (primitiveType->name == "double")
            return builder.CreateFPExt(value, newType);
          else
            return builder.CreateFPTrunc(value, newType);
        }

        else {
          if (willUnsigned)
            return builder.CreateFPToUI(value, newType);
          else
            return builder.CreateFPToSI(value, newType);
        }
      }

      else {
        if (willFloat) {
          if (wasUnsigned)
            return builder.CreateUIToFP(value, newType);
          else
            return builder.CreateSIToFP(value, newType);
        }

        if (willUnsigned)
          return builder.CreateZExtOrTrunc(value, newType);
        else
          return builder.CreateSExtOrTrunc(value, newType);
      }
    }

    throw std::runtime_error("cast not implemented");
  }

  virtual std::any visit(CallExpr &callExpr, std::any param) override {
    auto callee =
        std::any_cast<llvm::Value *>(callExpr.callee->accept(this, param));

    auto ptrType = llvm::IntegerType::get(ctx, 8)->getPointerTo();
    auto structType = llvm::StructType::get(ptrType, ptrType);
    auto ep = builder.CreateStructGEP(structType, callee, 0);
    auto callCtx = builder.CreateLoad(ptrType, ep);

    ep = builder.CreateStructGEP(structType, callee, 1);
    auto realCallee = (llvm::Value *)builder.CreateLoad(ptrType, ep);

    // realCallee = builder.CreatePointerCast(realCallee, calleeType);

    std::vector<llvm::Value *> llvmArgs;
    llvmArgs.push_back(callCtx);
    for (auto &arg : callExpr.args) {
      llvmArgs.push_back(
          std::any_cast<llvm::Value *>(arg->accept(this, param)));
    }

    auto calleeType =
        (llvm::FunctionType *)getLLVMType(ctx, callExpr.callee->type);
    return (llvm::Value *)builder.CreateCall(calleeType, realCallee, llvmArgs);
  }

  virtual std::any visit(Identifier &identifier, std::any param) override {
    auto value = symbolMap[identifier.symbol.get()];

    if (rvalueForLValue) {
      builder.CreateStore(rvalueForLValue, value);
      return nullptr;
    }

    else {
      if (functions.count(value))
        return wrapCallee(value);
      if (utils::is<FunctionType>(identifier.type.get()))
        return value;

      auto loadType = getLLVMType(ctx, identifier.type);
      return (llvm::Value *)builder.CreateLoad(loadType, value);
    }
  }

  virtual std::any visit(IntValue &intValue, std::any param) override {
    return (llvm::Value *)llvm::ConstantInt::get(
        getLLVMType(ctx, intValue.type), intValue.value);
  }

  virtual std::any visit(FloatValue &floatValue, std::any param) override {
    return (llvm::Value *)llvm::ConstantFP::get(
        getLLVMType(ctx, floatValue.type), floatValue.value);
  }

  virtual std::any visit(RetStmt &retStmt, std::any param) override {
    auto value =
        std::any_cast<llvm::Value *>(retStmt.value->accept(this, param));

    builder.CreateStore(value, retStore);
    builder.CreateBr(retBlock);

    return std::any();
  }

  virtual std::any visit(IfStmt &ifStmt, std::any param) override {
    auto cond =
        std::any_cast<llvm::Value *>(ifStmt.condition->accept(this, param));

    auto fn = builder.GetInsertBlock()->getParent();
    auto ifBlock = llvm::BasicBlock::Create(ctx, createLabel(), fn);
    auto elseBlock = llvm::BasicBlock::Create(ctx, createLabel(), fn);

    llvm::BasicBlock *endBlock = nullptr;
    if (!ifStmt.alywasReturns())
      endBlock = llvm::BasicBlock::Create(ctx, createLabel(), fn);

    builder.CreateCondBr(cond, ifBlock, elseBlock);

    builder.SetInsertPoint(ifBlock);
    ifStmt.ifBody->accept(this, param);
    if (endBlock && !ifStmt.ifBody->alywasReturns())
      builder.CreateBr(endBlock);

    builder.SetInsertPoint(elseBlock);
    ifStmt.elseBody->accept(this, param);
    if (endBlock && !ifStmt.elseBody->alywasReturns())
      builder.CreateBr(endBlock);

    if (endBlock && !ifStmt.alywasReturns())
      builder.SetInsertPoint(endBlock);

    return std::any();
  }

  virtual std::any visit(WhileStmt &whileStmt, std::any param) override {
    auto fn = builder.GetInsertBlock()->getParent();
    auto condBlock = llvm::BasicBlock::Create(ctx, createLabel(), fn);
    auto whileBlock = llvm::BasicBlock::Create(ctx, createLabel(), fn);
    auto endBlock = llvm::BasicBlock::Create(ctx, createLabel(), fn);

    builder.CreateBr(condBlock);

    builder.SetInsertPoint(condBlock);
    auto cond =
        std::any_cast<llvm::Value *>(whileStmt.condition->accept(this, param));
    builder.CreateCondBr(cond, whileBlock, endBlock);

    builder.SetInsertPoint(whileBlock);
    whileStmt.body->accept(this, param);
    if (!whileStmt.body->alywasReturns())
      builder.CreateBr(condBlock);

    builder.SetInsertPoint(endBlock);

    return std::any();
  }

  virtual std::any visit(AssignStmt &assignStmt, std::any param) override {
    auto rvalue =
        std::any_cast<llvm::Value *>(assignStmt.rval->accept(this, param));

    rvalueForLValue = rvalue;
    auto lvalue = assignStmt.lval->accept(this, param);
    rvalueForLValue = nullptr;

    return std::any();
  }
};

static void runMPM(llvm::Module &mod) {
  llvm::PassBuilder passBuilder;

  llvm::ModuleAnalysisManager mam;
  llvm::CGSCCAnalysisManager gam;
  llvm::FunctionAnalysisManager fam;
  llvm::LoopAnalysisManager lam;

  llvm::ModulePassManager mpm;

  passBuilder.registerModuleAnalyses(mam);
  passBuilder.registerCGSCCAnalyses(gam);
  passBuilder.registerFunctionAnalyses(fam);
  passBuilder.registerLoopAnalyses(lam);

  passBuilder.crossRegisterProxies(lam, fam, gam, mam);

  mpm = passBuilder.buildModuleOptimizationPipeline(
      llvm::OptimizationLevel::O3, llvm::ThinOrFullLTOPhase::None);

  mpm.run(mod, mam);

  mam.clear();
  gam.clear();
  fam.clear();
  lam.clear();
}

static void writeToFile(llvm::LLVMContext &ctx, llvm::Module &mod,
                        llvm::IRBuilder<> &builder,
                        const std::string &outfile) {
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto target = LLVM_DEFAULT_TARGET_TRIPLE;

  mod.setTargetTriple(target);

  std::string err;
  const llvm::Target *t = llvm::TargetRegistry::lookupTarget(target, err);
  if (!t)
    throw std::runtime_error(err);

  llvm::TargetMachine *targetMachine =
      t->createTargetMachine(target, "", "", llvm::TargetOptions(),
                             // TODO: make configurable
                             llvm::Reloc::PIC_);

  mod.setDataLayout(targetMachine->createDataLayout());

  std::error_code ec;
  llvm::raw_fd_ostream dest(outfile, ec, llvm::sys::fs::OF_None);
  if (ec)
    throw std::runtime_error(ec.message());

  llvm::legacy::PassManager pm;

  auto &tm = (llvm::LLVMTargetMachine &)*targetMachine;

  pm.add(new llvm::TargetLibraryInfoWrapperPass());
  pm.add(new llvm::MachineModuleInfoWrapperPass(&tm));

  bool objResult = targetMachine->addPassesToEmitFile(
      pm, dest, nullptr, llvm::CodeGenFileType::ObjectFile);

  if (objResult)
    throw std::runtime_error("failed to produce " + outfile);

  pm.run(mod);
  dest.flush();
}

} // namespace

void compileModule(std::unique_ptr<ast::Module> &module,
                   const std::string &filename) {
  auto moduleId = filename;

  llvm::LLVMContext ctx;
  llvm::Module mod(moduleId, ctx);
  llvm::IRBuilder<> builder(ctx);

  std::map<Symbol *, llvm::Value *> symbolMap;
  std::set<llvm::Value *> functions;

  IRGenerator1 generator1(ctx, mod, builder, symbolMap, functions);
  module->accept(&generator1, nullptr);

  IRGenerator2 generator2(ctx, mod, builder, symbolMap, functions);
  for (auto &stmt : module->stmts) {
    if (utils::is<FnDecl>(stmt.get()))
      stmt->accept(&generator2, nullptr);
  }

  if (llvm::verifyModule(mod, &llvm::errs())) {
    mod.print(llvm::outs(), nullptr);
    llvm::outs().flush();
    throw std::runtime_error("Module verification failed");
  }

  runMPM(mod); // info: does not work, programs will malfunction

  mod.print(llvm::outs(), nullptr);
  llvm::outs().flush();

  // std::cout << "----------------------------------------------" << std::endl;
  // mod.print(llvm::outs(), nullptr);
  // llvm::outs().flush();

  writeToFile(ctx, mod, builder, filename + ".o");
}

} // namespace plsm