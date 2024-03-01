#pragma once

#include "AST/Base.h"
#include <cstdint>

namespace ast
{
  class NullValue : public Expr
  {
  public:
    NullValue(LOC_ARG) : Expr(location) {}
  };

  class IntValue : public Expr
  {
    const int64_t value;

  public:
    IntValue(LOC_ARG, int64_t value)
        : Expr(location), value(value) {}
  };

  class FloatValue : public Expr
  {
    const double value;

  public:
    FloatValue(LOC_ARG, double value)
        : Expr(location), value(value) {}
  };
}
