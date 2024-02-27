#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>

namespace ast
{
  typedef std::pair<std::string, Type *> FnArg;

  class FnDecl : public Stmt
  {
  public:
    FnDecl(const std::string &name, const std::vector<FnArg> &args, const Type *returnType, const Expr *body)
        : name(name), args(std::move(args)), returnType(returnType), body(body) {}

    ~FnDecl()
    {
      for (auto &arg : args)
        delete arg.second;

      delete returnType;
      delete body;
    }

    const std::string name;
    const std::vector<FnArg> args;
    const Type *returnType;
    const Expr *body;
  };
}