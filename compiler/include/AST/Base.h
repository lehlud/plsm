#pragma once

namespace ast
{
  class ASTNode
  {
  public:
    virtual ~ASTNode() = default;

    virtual bool isExpr() { return false; }
    virtual bool isStmt() { return false; }
    virtual bool isType() { return false; }
  };

  class Expr : public ASTNode
  {
  public:
    virtual bool isExpr() override { return true; }
  };

  class Stmt : public ASTNode
  {
  public:
    virtual bool isStmt() override { return true; }
  };

  class Type : public ASTNode
  {
  public:
    virtual bool isType() override { return true; }
  };
}