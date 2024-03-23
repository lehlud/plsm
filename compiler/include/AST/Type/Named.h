#pragma once

#include "AST/Base.h"
#include <string>

namespace plsm {
namespace ast {

class NamedType : public Type {
  const std::string name;

public:
  NamedType(LOC_ARG, const std::string &name) : Type(location), name(name) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<NamedType> fromJson(boost::json::value json);
};

} // namespace ast
} // namespace plsm
