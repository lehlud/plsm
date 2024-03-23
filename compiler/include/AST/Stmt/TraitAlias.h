#pragma once

#include "AST/Base.h"
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace plsm {
namespace ast {
class TraitAlias : public Stmt {
  const std::string alias;
  const std::string aliased;

public:
  TraitAlias(LOC_ARG, const std::string &alias, const std::string &aliased)
      : Stmt(location), alias(alias), aliased(aliased) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<TraitAlias> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
