#pragma once

#include "AST/Base.h"
#include <vector>
#include <memory>

namespace ast
{
  class CallExpr : public Expr
  {
    const std::unique_ptr<Expr> callee;
    const std::vector<std::unique_ptr<Expr>> args;

  public:
    CallExpr(LOC_ARG, std::unique_ptr<Expr> &callee, std::vector<std::unique_ptr<Expr>> &args)
        : Expr(location), callee(std::move(callee)), args(std::move(args)) {}

    virtual std::string str()
    {
      std::stringstream ss;

      ss << "CallExpr(";
      ss << "callee=" << callee->str();

      ss << "args=[";
      for (size_t i = 0; i < args.size(); i++)
      {
        ss << args[i]->str();
        if (i != args.size() - 1)
          ss << ", ";
      }
      ss << "]";

      ss << ")";

      return ss.str();
    }
  };
}