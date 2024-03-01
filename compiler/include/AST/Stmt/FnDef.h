#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>
#include <memory>

namespace ast
{
  class FnArg : public ASTNode
  {
    const std::string name;
    const std::unique_ptr<Type> type;

  public:
    FnArg(LOC_ARG, const std::string &name, std::unique_ptr<Type> &type)
        : ASTNode(location), name(name), type(std::move(type)) {}
  };

  class FnDef : public Stmt
  {
    const std::string name;
    const std::vector<std::unique_ptr<FnArg>> args;
    const std::unique_ptr<Type> returnType;
    const std::unique_ptr<Expr> body;

  public:
    FnDef(LOC_ARG, const std::string &name, std::vector<std::unique_ptr<FnArg>> &args, std::unique_ptr<Type> &returnType, std::unique_ptr<Expr> &body)
        : Stmt(location), name(name), args(std::move(args)), returnType(std::move(returnType)), body(std::move(body)) {}
  };
}