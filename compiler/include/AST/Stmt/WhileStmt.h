#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {
class WhileStmt : public Stmt {
public:
  const std::shared_ptr<Expr> condition;
  std::vector<std::shared_ptr<Stmt>> body;

  WhileStmt(LOC_ARG, Expr *condition, const std::vector<Stmt *> &body)
      : Stmt(sourceRange), condition(condition) {
    for (auto &stmt : body) {
      this->body.push_back(std::shared_ptr<Stmt>(stmt));
    }
  }

  virtual boost::json::value toJson() override;
  static WhileStmt *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};
} // namespace ast
} // namespace plsm
