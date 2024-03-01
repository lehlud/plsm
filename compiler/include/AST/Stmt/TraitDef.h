#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>
#include <memory>

namespace ast
{
  class FnDef;

  class TraitDef : public Stmt
  {
    const std::string name;
    const std::vector<std::unique_ptr<FnDef>> traits;

  public:
    TraitDef(LOC_ARG, const std::string &name, std::vector<std::unique_ptr<FnDef>> &traits)
        : Stmt(location), name(name), traits(std::move(traits)) {}
  };
}