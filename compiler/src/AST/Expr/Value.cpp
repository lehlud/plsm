#include "AST/Def.h"
#include <memory>

namespace plsm {

boost::json::value ast::NullValue::toJson() { return nullptr; }

std::unique_ptr<ast::NullValue>
ast::NullValue::fromJson(boost::json::value json) {
  return std::make_unique<ast::NullValue>(ast::Location::json());
}

boost::json::value ast::IntValue::toJson() { return value; }

std::unique_ptr<ast::IntValue>
ast::IntValue::fromJson(boost::json::value json) {
  return std::make_unique<ast::IntValue>(ast::Location::json(),
                                         json.as_int64());
}

boost::json::value ast::FloatValue::toJson() { return value; }

std::unique_ptr<ast::FloatValue>
ast::FloatValue::fromJson(boost::json::value json) {
  return std::make_unique<ast::FloatValue>(ast::Location::json(),
                                           json.as_double());
}

} // namespace plsm
