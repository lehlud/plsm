#pragma once

#include "AST/Base.h"
#include <cstddef>
#include <iomanip>
#include <memory>
#include <vector>
#include <sstream>

namespace plsm {
namespace ast {
class Import;

class Module : public ASTNode {
  const std::string name;
  const std::vector<std::unique_ptr<Import>> imports;
  const std::vector<std::unique_ptr<Stmt>> stmts;

public:
  Module(LOC_ARG, const std::string &name,
         std::vector<std::unique_ptr<Import>> &imports,
         std::vector<std::unique_ptr<Stmt>> &stmts)
      : ASTNode(location), name(name), imports(std::move(imports)),
        stmts(std::move(stmts)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<Module> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
