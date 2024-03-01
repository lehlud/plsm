#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>

namespace ast
{
  class PropExpr : public Expr
  {
    const std::unique_ptr<Expr> expr;
    const std::string property;

  public:
    PropExpr(LOC_ARG, std::unique_ptr<Expr> &expr, const std::string &property)
        : Expr(location), expr(std::move(expr)), property(property) {}

    virtual std::string str()
    {
      return (std::stringstream() << "PropExpr("
                                  << "expr=" << expr->str() << ", "
                                  << "property=" << property
                                  << ")")
          .str();
    }
  };
}