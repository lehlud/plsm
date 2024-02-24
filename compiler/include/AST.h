#pragma once

#include <string>

class Expr;

class ASTNode
{
};

class Stmt : public ASTNode
{
};

class Decl : public Stmt
{
public:
  Decl(const std::string &identifier, const Expr *value)
      : identifier(identifier), value(value) {}

  const std::string identifier;
  const Expr *value;
};

class Expr : public ASTNode
{
};

class IntValue : public Expr
{
public:
  const long value;
  IntValue(const long value) : value(value) {}
};

class FloatValue : public Expr
{
public:
  const double value;
  FloatValue(const double value) : value(value) {}
};

class Function : public Expr
{
};

class List : public Expr
{
};
