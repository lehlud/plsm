#include "AST/AST.h"
#include <memory>

namespace plsm {
namespace ast {
// boost::json::value NullValue::toJson() const { return nullptr; }

// std::unique_ptr<NullValue>
// NullValue::fromJson(boost::json::value json) {
//   return std::make_unique<NullValue>(SourceRange::json());
// }

boost::json::value IntValue::toJson() const { return value; }

IntValue *IntValue::fromJson(boost::json::value json) {
  return new IntValue(SourceRange::json(), json.as_int64());
}

boost::json::value FloatValue::toJson() const { return value; }

FloatValue *FloatValue::fromJson(boost::json::value json) {
  return new FloatValue(SourceRange::json(), json.as_double());
}

boost::json::value StringValue::toJson() const { return boost::json::value_from(value); }

StringValue *StringValue::fromJson(boost::json::value json) {
  return new StringValue(SourceRange::json(), json.as_string().c_str());
}

} // namespace ast
} // namespace plsm
