#pragma once

#include "AST/Base.h"
#include <string>
#include <vector>
#include <memory>

namespace ast
{
  class Closure : public Expr
  {
    const std::vector<std::string> args;
    const std::unique_ptr<Expr> body;

  public:
    Closure(LOC_ARG, std::vector<std::string> &args, std::unique_ptr<Expr> &body)
        : Expr(location), args(std::move(args)), body(std::move(body)) {}

    virtual std::string str()
    {
      std::stringstream ss;

      ss << "CallExpr(";
      ss << "args=[";
      for (size_t i = 0; i < args.size(); i++)
      {
        ss << args[i];
        if (i != args.size() - 1)
          ss << ", ";
      }
      ss << "], ";
      ss << "body=" << body->str();
      ss << ")";

      return ss.str();
    }
  };
}