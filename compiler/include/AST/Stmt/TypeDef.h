#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>
#include <memory>

namespace ast
{
  class FnDecl;

  typedef std::pair<std::string, std::unique_ptr<Type>> TypeAttr;

  class TypeDef : public Stmt
  {
    const std::string name;
    const std::vector<TypeAttr> attrs;
    const std::vector<std::unique_ptr<FnDecl>> members;

  public:
    TypeDef(const std::string &name, std::vector<TypeAttr> &attrs, std::vector<std::unique_ptr<FnDecl>> &members)
        : name(name), attrs(std::move(attrs)), members(std::move(members)) {}
  };
}