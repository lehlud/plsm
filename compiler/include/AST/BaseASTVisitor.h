#pragma once

#include "AST/AST.h"

namespace plsm {
namespace ast {

class BaseASTVisitor : public ASTVisitor {
public:
  BaseASTVisitor() : ASTVisitor() {};

  virtual std::any visit(BinExpr &binExpr, std::any param) override {
    binExpr.lhs->accept(this, param);
    binExpr.rhs->accept(this, param);
    return std::any();
  }

  virtual std::any visit(CallExpr &callExpr, std::any param) override {
    callExpr.callee->accept(this, param);

    for (auto &arg : callExpr.args) {
      arg->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(CastExpr &castExpr, std::any param) override {
    castExpr.value->accept(this, param);
    castExpr.typeName->accept(this, param);
    return std::any();
  }

  virtual std::any visit(Identifier &identifier, std::any param) override {
    return std::any();
  }

  virtual std::any visit(LambdaExpr &lambdaExpr, std::any param) override {
    for (auto &param : lambdaExpr.params) {
      param->accept(this, param);
    }

    for (auto &body : lambdaExpr.body) {
      body->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(UnExpr &unExpr, std::any param) override {
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
      stmt->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(AssignStmt &assignStmt, std::any param) override {
    assignStmt.lval->accept(this, param);
    assignStmt.rval->accept(this, param);
    return std::any();
  }

  virtual std::any visit(ExprStmt &exprStmt, std::any param) override {
    exprStmt.expr->accept(this, param);
    return std::any();
  }

  virtual std::any visit(FnParam &fnParam, std::any param) override {
    fnParam.typeName->accept(this, param);
    return std::any();
  }

  virtual std::any visit(FnDecl &fnDecl, std::any param) override {
    for (auto &param : fnDecl.params) {
      param->accept(this, param);
    }

    for (auto &body : fnDecl.body) {
      body->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(IfStmt &ifStmt, std::any param) override {
    ifStmt.condition->accept(this, param);

    for (auto &ifBody : ifStmt.ifBody) {
      ifBody->accept(this, param);
    }

    for (auto &elseBody : ifStmt.elseBody) {
      elseBody->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(RetStmt &retStmt, std::any param) override {
    retStmt.value->accept(this, param);
    return std::any();
  }

  virtual std::any visit(VarDecl &varDecl, std::any param) override {
    varDecl.typeName->accept(this, param);
    return std::any();
  }

  virtual std::any visit(WhileStmt &whileStmt, std::any param) override {
    whileStmt.condition->accept(this, param);

    for (auto &body : whileStmt.body) {
      body->accept(this, param);
    }

    return std::any();
  }

  virtual std::any visit(PrimitiveTypeName &primitiveTypeName,
                         std::any param) override {
    return std::any();
  }
};

} // namespace ast
} // namespace plsm