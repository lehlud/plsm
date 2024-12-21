#pragma once

#include "AST/Base.h"

namespace plsm {
namespace ast {

class PrimitiveType : public Type {
  const std::string name;

public:
  PrimitiveType(const std::string &name) : Type(), name(name) {}

  bool operator==(const PrimitiveType &other) { return name == other.name; }
  bool operator!=(const PrimitiveType &other) { return !(*this == other); }

  virtual boost::json::value toJson() override;
  static PrimitiveType *fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
