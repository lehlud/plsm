#pragma once

#include "AST/AST.h"

namespace plsm {
namespace ast {

class BaseASTVisitor : public ASTVisitor {
public:
  BaseASTVisitor() : ASTVisitor() {};

  virtual std::any visit(BinExpr &binExpr, std::any param) override {
    if (binExpr.lhs.get())
      binExpr.lhs->accept(this, param);
    if (binExpr.rhs.get())
      binExpr.rhs->accept(this, param);
    return std::any();
  }

  virtual std::any visit(CallExpr &callExpr, std::any param) override {
    if (callExpr.callee.get())
      callExpr.callee->accept(this, param);

    for (auto &arg : callExpr.args) {
      if (arg.get())
        arg->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(CastExpr &castExpr, std::any param) override {
    if (castExpr.value.get())
      castExpr.value->accept(this, param);
    if (castExpr.typeName.get())
      castExpr.typeName->accept(this, param);
    return std::any();
  }

  virtual std::any visit(Identifier &identifier, std::any param) override {
    return std::any();
  }

  virtual std::any visit(LambdaExpr &lambdaExpr, std::any param) override {
    if (lambdaExpr.returnTypeName.get())
      lambdaExpr.returnTypeName->accept(this, param);

    for (auto &p : lambdaExpr.params) {
      if (p.get())
        p->accept(this, param);
    }

    if (lambdaExpr.body.get())
      lambdaExpr.body->accept(this, param);

    return std::any();
  }

  virtual std::any visit(UnExpr &unExpr, std::any param) override {
    if (unExpr.expr.get())
      unExpr.expr->accept(this, param);
    return std::any();
  }

  virtual std::any visit(IntValue &intValue, std::any param) override {
    return std::any();
  }

  virtual std::any visit(FloatValue &floatValue, std::any param) override {
    return std::any();
  }

  virtual std::any visit(Import &import, std::any param) override {
    return std::any();
  }

  virtual std::any visit(Module &module, std::any param) override {
    for (auto &import : module.imports) {
      import->accept(this, param);
    }

    for (auto &stmt : module.stmts) {
      if (stmt.get())
        stmt->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(AssignStmt &assignStmt, std::any param) override {
    if (assignStmt.lval.get())
      assignStmt.lval->accept(this, param);
    if (assignStmt.rval.get())
      assignStmt.rval->accept(this, param);
    return std::any();
  }

  virtual std::any visit(Block &block, std::any param) override {
    for (auto &stmt : block.stmts) {
      if (stmt.get())
        stmt->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(ExprStmt &exprStmt, std::any param) override {
    if (exprStmt.expr.get())
      exprStmt.expr->accept(this, param);
    return std::any();
  }

  virtual std::any visit(FnParam &fnParam, std::any param) override {
    if (fnParam.typeName.get())
      fnParam.typeName->accept(this, param);
    return std::any();
  }

  virtual std::any visit(FnDecl &fnDecl, std::any param) override {
    if (fnDecl.returnTypeName.get())
      fnDecl.returnTypeName->accept(this, param);

    for (auto &p : fnDecl.params) {
      if (p.get())
        p->accept(this, param);
    }

    if (fnDecl.body.get())
      fnDecl.body->accept(this, param);

    return std::any();
  }

  virtual std::any visit(IfStmt &ifStmt, std::any param) override {
    if (ifStmt.condition.get())
      ifStmt.condition->accept(this, param);
    if (ifStmt.ifBody.get())
      ifStmt.ifBody->accept(this, param);
    if (ifStmt.elseBody.get())
      ifStmt.elseBody->accept(this, param);

    return std::any();
  }

  virtual std::any visit(RetStmt &retStmt, std::any param) override {
    if (retStmt.value.get())
      retStmt.value->accept(this, param);
    return std::any();
  }

  virtual std::any visit(VarDecl &varDecl, std::any param) override {
    if (varDecl.typeName.get())
      varDecl.typeName->accept(this, param);
    return std::any();
  }

  virtual std::any visit(WhileStmt &whileStmt, std::any param) override {
    if (whileStmt.condition.get())
      whileStmt.condition->accept(this, param);
    if (whileStmt.body.get())
      whileStmt.body->accept(this, param);

    return std::any();
  }

  virtual std::any visit(NamedTypeName &namedTypeName,
                         std::any param) override {
    return std::any();
  }
};

} // namespace ast
} // namespace plsm