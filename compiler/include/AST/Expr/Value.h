#pragma once

#include "AST/Base.h"
#include <cfloat>
#include <cstdint>
#include <string>

namespace plsm {
namespace ast {
// class NullValue : public Expr {
// public:
//   NullValue(LOC_ARG) : Expr(sourceRange) {}

//   virtual boost::json::value toJson() override;
//   static std::unique_ptr<NullValue> fromJson(boost::json::value json);
// };

class IntValue : public Expr {
public:
  const std::int64_t value;

  IntValue(LOC_ARG, int64_t value) : Expr(sourceRange), value(value) {}

  virtual boost::json::value toJson() override;
  static IntValue *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

class FloatValue : public Expr {
public:
  const std::double_t value;

  FloatValue(LOC_ARG, double value) : Expr(sourceRange), value(value) {}

  virtual boost::json::value toJson() override;
  static FloatValue *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm
