#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>

namespace ast
{
  class FnDecl;

  class TraitDecl : public Stmt
  {
  public:
    TraitDecl(const std::string &name, const std::vector<FnDecl *> &traits)
        : name(name), traits(std::move(traits)) {}
    ~TraitDecl()
    {
      for (auto &trait : traits)
        delete trait;
    }

    const std::string name;
    const std::vector<FnDecl *> traits;
  };
}