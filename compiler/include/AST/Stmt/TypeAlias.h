#pragma once

#include "AST/Base.h"
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace plsm {
namespace ast {
class TypeAlias : public Stmt {
  const std::string alias;
  const std::unique_ptr<Type> type;

public:
  TypeAlias(LOC_ARG, const std::string &alias, std::unique_ptr<Type> &type)
      : Stmt(location), alias(alias), type(std::move(type)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<TypeAlias> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
