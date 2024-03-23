#pragma once

#include "AST/Base.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {
class ClosureType : public Type {
  const std::vector<std::unique_ptr<Type>> from;
  const std::unique_ptr<Type> to;

public:
  ClosureType(LOC_ARG, std::vector<std::unique_ptr<Type>> &from,
              std::unique_ptr<Type> &to)
      : Type(location), from(std::move(from)), to(std::move(to)) {}

  
  virtual boost::json::value toJson() override;
  static std::unique_ptr<ClosureType> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
