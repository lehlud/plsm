#pragma once

#include "AST/Base.h"
#include <vector>
#include <memory>

namespace ast
{
  class Tuple : public Expr
  {
    const std::vector<std::unique_ptr<Expr>> values;

  public:
    Tuple(LOC_ARG, std::vector<std::unique_ptr<Expr>> &values)
        : Expr(location), values(std::move(values)) {}
  };
}