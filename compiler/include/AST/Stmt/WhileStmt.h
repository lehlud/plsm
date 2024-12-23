#pragma once

#include "AST/Base.h"
#include <memory>

namespace plsm {
namespace ast {
class WhileStmt : public Stmt {
public:
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Block> body;

  WhileStmt(LOC_ARG, std::unique_ptr<Expr> condition,
            std::unique_ptr<Block> body)
      : Stmt(sourceRange), condition(std::move(condition)),
        body(std::move(body)) {}

  virtual boost::json::value toJson() const override;
  static WhileStmt *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
