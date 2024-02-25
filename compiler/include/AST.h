#pragma once

#include <string>
#include <vector>

class Expr;
class Type;

typedef std::pair<std::string, Type *> FnArg;

class ASTNode
{
};

class Stmt : public ASTNode
{
};

class Module : public ASTNode
{
public:
  Module(const std::vector<Stmt *> &stmts) : stmts(std::move(stmts)) {}
  const std::vector<Stmt *> stmts;
};

class ValDecl : public Stmt
{
public:
  ValDecl(const std::string &name, const Type *type, const Expr *value)
      : name(name), type(type), value(value) {}

  const std::string name;
  const Type *type;
  const Expr *value;
};

class FnDecl : public Stmt
{
public:
  FnDecl(const std::string &name, const std::vector<FnArg> &args, const Type *returnType, const Expr *body)
      : name(name), args(std::move(args)), returnType(returnType), body(body) {}

  const std::string name;
  const std::vector<FnArg> args;
  const Type *returnType;
  const Expr *body;
};

class Type : public ASTNode
{
};

class TupleType : public Type
{
public:
  TupleType(const std::vector<Type *> &types) : types(types) {}
  const std::vector<Type *> types;
};

class FunctionType : public Type
{
public:
  FunctionType(const std::vector<Type *> &from, const Type *to)
      : from(std::move(from)), to(to) {}

  const std::vector<Type *> from;
  const Type *to;
};

class NamedType : public Type
{
public:
  NamedType(const std::string &name) : name(name) {}
  const std::string name;
};

class Expr : public ASTNode
{
};

class CallExpr : public Expr
{
public:
  CallExpr(const Expr *callee, const std::vector<Expr *> &args)
      : callee(callee), args(std::move(args)) {}

  const Expr *callee;
  const std::vector<Expr *> args;
};

class UnaryExpr : public Expr
{
public:
  UnaryExpr(const std::string &op, const Expr *expr)
      : op(op), expr(expr) {}

  const std::string op;
  const Expr *expr;
};

class BinExpr : public Expr
{
public:
  BinExpr(const Expr *left, const std::string &op, const Expr *right)
      : left(left), op(op), right(right) {}

  const Expr *left;
  const std::string op;
  const Expr *right;
};

class PrefExpr : public Expr
{
public:
  PrefExpr(const Expr *expr) : expr(expr) {}

  const Expr *expr;
};

class Identifier : public Expr
{
public:
  Identifier(const std::string &name) : name(name) {}

  const std::string name;
};

class IntValue : public Expr
{
public:
  IntValue(const long value) : value(value) {}

  const long value;
};

class FloatValue : public Expr
{
public:
  FloatValue(const double value) : value(value) {}

  const double value;
};
