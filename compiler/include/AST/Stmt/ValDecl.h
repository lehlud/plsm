#pragma once

#include "AST/Base.h"
#include <string>

namespace ast
{
  class ValDecl : public Stmt
  {
  public:
    ValDecl(const std::string &name, const Type *type, const Expr *value)
        : name(name), type(type), value(value) {}

    ~ValDecl()
    {
      delete type;
      delete value;
    }

    const std::string name;
    const Type *type;
    const Expr *value;
  };
}