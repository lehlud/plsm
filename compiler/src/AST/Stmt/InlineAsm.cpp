#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value InlineAsmConstraint::toJson() const {
  return {
      {"@type", "InlineAsmConstraint"},
      {"constraint", constraint},
      {"variable", variable},
  };
}

InlineAsmConstraint *InlineAsmConstraint::fromJson(boost::json::value json) {
  auto constraint =
      getJsonValue<InlineAsmConstraint, std::string>(json, "constraint");
  auto variable =
      getJsonValue<InlineAsmConstraint, std::string>(json, "variable");
  return new InlineAsmConstraint(SourceRange::json(), constraint, variable);
}

boost::json::value InlineAsm::toJson() const {
  return {
      {"@type", "InlineAsm"},
      {"code", code},
      {"outputs", utils::mapToJson(outputs)},
      {"inputs", utils::mapToJson(inputs)},
      {"clobbers", clobbers},
  };
}

InlineAsm *InlineAsm::fromJson(boost::json::value json) {
  auto name = getJsonValue<InlineAsm, std::string>(json, "name");
  auto outputs =
      fromJsonVector<InlineAsm, InlineAsmConstraint>(json, "outputs");
  auto inputs = fromJsonVector<InlineAsm, InlineAsmConstraint>(json, "inputs");
  auto clobbers =
      getJsonValue<InlineAsm, std::vector<std::string>>(json, "clobbers");
  return new InlineAsm(SourceRange::json(), name, std::move(outputs),
                       std::move(inputs), clobbers);
}

} // namespace ast
} // namespace plsm
