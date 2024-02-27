#pragma once

namespace ast
{
  class ASTNode
  {
  public:
    virtual ~ASTNode() = default;
  };

  class Expr : public ASTNode
  {
  };

  class Stmt : public ASTNode
  {
  };

  class Type : public ASTNode
  {
  };
}