#pragma once

#include "AST/Base.h"
#include <string>

namespace plsm {
namespace ast {
class Identifier : public Expr {
  const std::string name;

public:
  Identifier(LOC_ARG, const std::string &name) : Expr(location), name(name) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<Identifier> fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
