#pragma once

#include "AST/Base.h"
#include <vector>
#include <string>

namespace ast
{
  class FnDecl;

  typedef std::pair<std::string, Type *> TypeAttr;

  class TypeDecl : public Stmt
  {
  public:
    TypeDecl(const std::string &name, const std::vector<TypeAttr> &attrs, const std::vector<FnDecl *> &members)
        : name(name), attrs(std::move(attrs)), members(std::move(members)) {}
    ~TypeDecl()
    {
      for (auto &attr : attrs)
        delete attr.second;

      for (auto &member : members)
        delete member;
    }

    const std::string name;
    const std::vector<TypeAttr> attrs;
    const std::vector<FnDecl *> members;
  };
}