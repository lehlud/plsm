#pragma once

#include "AST/Base.h"
#include <memory>
#include <sstream>
#include <vector>

namespace plsm {
namespace ast {

class Tuple : public Expr {
  const std::vector<std::unique_ptr<Expr>> values;

public:
  Tuple(LOC_ARG, std::vector<std::unique_ptr<Expr>> &values)
      : Expr(location), values(std::move(values)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<Tuple> fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
