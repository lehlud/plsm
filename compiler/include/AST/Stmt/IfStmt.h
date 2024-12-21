#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {

class IfStmt : public Stmt {
  const std::shared_ptr<Expr> condition;
  std::vector<std::shared_ptr<Stmt>> ifBody, elseBody;

public:
  IfStmt(LOC_ARG, Expr *condition, const std::vector<Stmt *> &ifBody,
         const std::vector<Stmt *> &elseBody)
      : Stmt(sourceRange), condition(condition) {
    for (auto &stmt : ifBody) {
      this->ifBody.push_back(std::shared_ptr<Stmt>(stmt));
    }

    for (auto &stmt : elseBody) {
      this->elseBody.push_back(std::shared_ptr<Stmt>(stmt));
    }
  }

  virtual boost::json::value toJson() override;
  static IfStmt *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
