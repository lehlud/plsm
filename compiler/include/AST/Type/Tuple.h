#pragma once

#include "AST/Def.h"
#include <memory>
#include <vector>

namespace plsm {
namespace ast {
class TupleType : public Type {
  const std::vector<std::unique_ptr<Type>> types;

public:
  TupleType(LOC_ARG, std::vector<std::unique_ptr<Type>> &types)
      : Type(location), types(std::move(types)) {}

  
  virtual boost::json::value toJson() override;
  static std::unique_ptr<TupleType> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
