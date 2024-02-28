#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>
#include <memory>

namespace ast
{
  typedef std::pair<std::string, std::unique_ptr<Type>> FnArg;

  class FnDecl : public Stmt
  {
    const std::string name;
    const std::vector<FnArg> args;
    const std::unique_ptr<Type> returnType;
    const std::unique_ptr<Expr> body;

  public:
    FnDecl(const std::string &name, std::vector<FnArg> &args, std::unique_ptr<Type> &returnType, std::unique_ptr<Expr> &body)
        : name(name), args(std::move(args)), returnType(std::move(returnType)), body(std::move(body)) {}
  };
}