#pragma once

#include "AST/Base.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace plsm {
namespace ast {
class FnArg;

class Closure : public Expr {
  const std::vector<std::unique_ptr<FnArg>> args;
  const std::unique_ptr<Expr> body;

public:
  Closure(LOC_ARG, std::vector<std::unique_ptr<FnArg>> &args, std::unique_ptr<Expr> &body)
      : Expr(location), args(std::move(args)), body(std::move(body)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<Closure> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
