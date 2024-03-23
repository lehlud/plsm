#pragma once

#include "AST/Base.h"
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace plsm {
namespace ast {
class FnDef;
class TraitDef : public Stmt {
  const std::string name;
  const std::vector<std::unique_ptr<FnDef>> traits;

public:
  TraitDef(LOC_ARG, const std::string &name,
           std::vector<std::unique_ptr<FnDef>> &traits)
      : Stmt(location), name(name), traits(std::move(traits)) {}

  virtual boost::json::value toJson() override;
  static std::unique_ptr<TraitDef> fromJson(boost::json::value json);
};
} // namespace ast
} // namespace plsm
