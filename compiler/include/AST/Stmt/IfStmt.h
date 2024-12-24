#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {

class IfStmt : public Stmt {
public:
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Block> ifBody, elseBody;

  IfStmt(LOC_ARG, std::unique_ptr<Expr> condition,
         std::unique_ptr<Block> ifBody, std::unique_ptr<Block> elseBody)
      : Stmt(sourceRange), condition(std::move(condition)),
        ifBody(std::move(ifBody)), elseBody(std::move(elseBody)) {}

  virtual boost::json::value toJson() const override;
  static IfStmt *fromJson(boost::json::value json);

  virtual bool alywasReturns() const override {
    return (ifBody.get() && ifBody->alywasReturns()) ||
           (elseBody.get() && elseBody->alywasReturns());
  }

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
