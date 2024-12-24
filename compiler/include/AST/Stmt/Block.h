#pragma once

#include "AST/Base.h"

namespace plsm {
namespace ast {

class Block : public ASTNode {
public:
  std::vector<std::unique_ptr<Stmt>> stmts;

  Block(LOC_ARG, std::vector<std::unique_ptr<Stmt>> stmts)
      : ASTNode(sourceRange), stmts(std::move(stmts)) {}

  virtual boost::json::value toJson() const override;
  static Block *fromJson(boost::json::value json);

  bool alywasReturns() const {
    for (auto &stmt : stmts)
      if (stmt.get() && stmt->alywasReturns())
        return true;
    return false;
  }

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
