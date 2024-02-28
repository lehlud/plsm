#pragma once

#include "AST/Base.h"
#include <cstdint>

namespace ast
{
  class IntValue : public Expr
  {
    const int64_t value;

  public:
    IntValue(int64_t value) : value(value) {}
  };

  class FloatValue : public Expr
  {
    const double value;

  public:
    FloatValue(double value) : value(value) {}
  };
}
