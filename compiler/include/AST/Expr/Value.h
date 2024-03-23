#pragma once

#include "AST/Base.h"
#include <string>
#include <cstdint>
#include <cfloat>

namespace plsm {
namespace ast {
class NullValue : public Expr {
public:
  NullValue(LOC_ARG) : Expr(location) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<NullValue> fromJson(boost::json::value json);
};

class IntValue : public Expr {
  const std::int64_t value;

public:
  IntValue(LOC_ARG, int64_t value) : Expr(location), value(value) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<IntValue> fromJson(boost::json::value json);
};

class FloatValue : public Expr {
  const std::double_t value;

public:
  FloatValue(LOC_ARG, double value) : Expr(location), value(value) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<FloatValue> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
