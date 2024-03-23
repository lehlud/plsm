#pragma once

#include "AST/Base.h"
#include <memory>
#include <string>
#include <vector>

namespace plsm {
namespace ast {
class FnDef;
class FnArg;

class TypeDef : public Stmt {
  const std::string name;
  const std::vector<std::unique_ptr<FnArg>> attrs;
  const std::vector<std::unique_ptr<FnDef>> members;

public:
  TypeDef(LOC_ARG, const std::string &name,
          std::vector<std::unique_ptr<FnArg>> &attrs,
          std::vector<std::unique_ptr<FnDef>> &members)
      : Stmt(location), name(name), attrs(std::move(attrs)),
        members(std::move(members)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<TypeDef> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
