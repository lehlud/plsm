#pragma once

#include "AST/Base.h"
#include <string>
#include <memory>

namespace ast
{
  class ValDecl : public Stmt
  {
    const std::string name;
    const std::unique_ptr<Type> type;
    const std::unique_ptr<Expr> value;

  public:
    ValDecl(LOC_ARG, const std::string &name, std::unique_ptr<Type> &type, std::unique_ptr<Expr> &value)
        : Stmt(location), name(name), type(std::move(type)), value(std::move(value)) {}
  };
}