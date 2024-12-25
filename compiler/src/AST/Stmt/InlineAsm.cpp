#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value InlineAsmConstraint::toJson() const {
  return {
      {"@type", "InlineAsmConstraint"},
      {"constraint", constraint},
      {"value", value->toJson()},
  };
}

InlineAsmConstraint *InlineAsmConstraint::fromJson(boost::json::value json) {
  auto constraint = getJsonValue<InlineAsmConstraint, std::string>(json, "constraint");
  auto value = fromJsonProperty<InlineAsmConstraint, Expr>(json, "value");
  return new InlineAsmConstraint(SourceRange::json(), constraint, std::move(value));
}

boost::json::value InlineAsm::toJson() const {
  boost::json::array jsonClobbers;

  return {
      {"@type", "InlineAsm"},
      {"code", code},
      {"outputs", utils::mapToJson(outputs)},
      {"inputs", utils::mapToJson(inputs)},
      {"clobbers", utils::mapToJson(clobbers, [](const std::string &clobber) { return clobber; })},
  };
}

InlineAsm *InlineAsm::fromJson(boost::json::value json) {
  auto name = getJsonValue<InlineAsm, std::string>(json, "name");
  auto outputs = fromJsonVector<InlineAsm, InlineAsmConstraint>(json, "outputs");
  auto inputs = fromJsonVector<InlineAsm, InlineAsmConstraint>(json, "inputs");
  auto clobbers = getJsonValue<InlineAsm, std::vector<std::string>>(json, "clobbers");
  return new InlineAsm(SourceRange::json(), name, std::move(outputs), std::move(inputs), std::move(clobbers));
}

} // namespace ast
} // namespace plsm
