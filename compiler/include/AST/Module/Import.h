#pragma once

#include "AST/Base.h"
#include <iomanip>
#include <sstream>
#include <string>

namespace plsm {
namespace ast {
class Import : public ASTNode {
  const std::string moduleName;

public:
  Import(LOC_ARG, const std::string &moduleName)
      : ASTNode(location), moduleName(moduleName) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<Import> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
