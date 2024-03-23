#pragma once

#include "AST/Base.h"
#include <memory>
#include <sstream>
#include <vector>

namespace plsm {
namespace ast {

class CallExpr : public Expr {
  const std::unique_ptr<Expr> callee;
  const std::vector<std::unique_ptr<Expr>> args;

public:
  CallExpr(LOC_ARG, std::unique_ptr<Expr> &callee,
           std::vector<std::unique_ptr<Expr>> &args)
      : Expr(location), callee(std::move(callee)), args(std::move(args)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<CallExpr> fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
