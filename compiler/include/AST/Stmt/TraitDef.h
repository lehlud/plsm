#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>
#include <memory>

namespace ast
{
  class FnDecl;

  class TraitDef : public Stmt
  {
    const std::string name;
    const std::vector<std::unique_ptr<FnDecl>> traits;

  public:
    TraitDef(const std::string &name, std::vector<std::unique_ptr<FnDecl>> &traits)
        : name(name), traits(std::move(traits)) {}
  };
}