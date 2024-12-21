#include "AST/AST.h"
#include <memory>

namespace plsm {
namespace ast {
// boost::json::value NullValue::toJson() { return nullptr; }

// std::unique_ptr<NullValue>
// NullValue::fromJson(boost::json::value json) {
//   return std::make_unique<NullValue>(SourceRange::json());
// }

boost::json::value IntValue::toJson() { return value; }

IntValue *IntValue::fromJson(boost::json::value json) {
  return new IntValue(SourceRange::json(), json.as_int64());
}

boost::json::value FloatValue::toJson() { return value; }

FloatValue *FloatValue::fromJson(boost::json::value json) {
  return new FloatValue(SourceRange::json(), json.as_double());
}

} // namespace ast
} // namespace plsm
