#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>
#include <vector>

namespace plsm {
namespace ast {
class GenericType : public Type {
  const std::string name;
  const std::vector<std::unique_ptr<Type>> types;

public:
  GenericType(LOC_ARG, const std::string &name,
              std::vector<std::unique_ptr<Type>> &types)
      : Type(location), name(name), types(std::move(types)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<GenericType> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
