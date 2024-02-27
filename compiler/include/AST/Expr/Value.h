#pragma once

#include "AST/Base.h"
#include <cstdint>

namespace ast
{
  class IntValue : public Expr
  {
  public:
    IntValue(int64_t value) : value(value) {}
    const int64_t value;
  };

  class FloatValue : public Expr
  {
  public:
    FloatValue(double value) : value(value) {}
    const double value;
  };
}
